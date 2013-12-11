#include "shm_core.h"
#include "shm_rr_queue.h"
#include "shm_circular_array.h"
#include "shm_inner_func.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>


void *get_shm_queue_handle(int self_id)
{
	shm_queue_handle_t *handle = NULL;
	handle = (shm_queue_handle_t*)calloc(1,sizeof(shm_queue_handle_t));
	if(NULL != handle)
	{
		handle->self_id = self_id;
	}

	handle->who_prior = READ_PRIO;

	return	(void*)handle;
}



int set_shm_queue_handle(void *handle, int type, int value)
{
	int ret = SHM_OK;
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	if(NULL == shmq_handle)
	{
		return SHM_HANDLE_NULL;
	}
	
	switch(type)
	{
		case SHMQ_PARAMETER_BLOCK_NUM:
		{
			shmq_handle->para_queue_size = value;
			break;
		}
		case SHMQ_PARAMETER_POOL_SIZE:
		{
			shmq_handle->para_pool_size = value;
			break;
		}
		case SHMQ_PARAMETER_OP_MODE:
		{	
			if(value == OP_MODE_READ || value == OP_MODE_WRITE)
			{
				shmq_handle->op_mode = value;
			}
			else
			{
				ret = SHM_UNKNOWN_OP;
			}
			break;
		}
		case SHMQ_PARAMETER_WHO_PRIO:
		{	
			if(check_write_id(shmq_handle->self_id))			
			{			
				if(value == READ_PRIO  || value == WRITE_PRIO)
				{
					shmq_handle->who_prior = value;
				}
				else
				{
					ret = SHM_UNKNOWN_PRIO;
				}
			}
			else
			{
				ret = SHM_NO_AUTHORITY;
			}
			break;
		}
		default:
		{
			ret = SHM_UNKNOWN_TYPE;
			break;
		}
	}
	return ret;
}




int open_shm_queue(void*handle, key_t key_id)
{
	int ret;
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	if(NULL == shmq_handle)
	{
		return SHM_HANDLE_NULL;
	}
	
	if(shmq_handle->op_mode == OP_MODE_READ)
	{
		ret = check_read_id(shmq_handle->self_id);
		
		if(ret < 0)
		{
			return  SHM_NO_AUTHORITY;
		}

		ret = shmget(key_id, 0, 0);
		
		if(ret < 0)
		{        
			return SHM_NO_SHM;
		}
		
		else
		{	
			char *sh_mem = NULL;
			sh_mem = (char*)shmat(ret , NULL , 0);
			if(NULL == sh_mem)
			{
				return SHM_NO_SHM;
			}
			
			init_read_handle_info(shmq_handle,key_id, sh_mem);
		}
	}

	else if(shmq_handle->op_mode == OP_MODE_WRITE)
	{      
		ret = check_write_id(shmq_handle->self_id);
		
		if(!ret)
		{
			return SHM_NO_AUTHORITY;
		}  
		
		ret = shmget(key_id,0,0); 
		if(ret < 0)
		{

			int mem_len = 	sizeof(shm_queue_info_t) +
					sizeof(circular_array_t) +
					shmq_handle->para_pool_size +
					shmq_handle->para_queue_size * sizeof(block_info_t);
			
			shmq_handle->shm_fd = shmget(key_id,mem_len,0666 | IPC_CREAT);
			
			if(shmq_handle->shm_fd < 0)
			{
				return SHM_CREATE_SHMEM_FAIL;
			}
			char *sh_mem= (char*)shmat(shmq_handle->shm_fd, NULL, 0);
			init_shm_info(sh_mem, shmq_handle);
			init_write_handle_info(shmq_handle, key_id, sh_mem);
		}
		else
		{
			return SHM_ALREADY_EXIST;				
		}
	}
	
	else
	{
		return SHM_UNKNOWN_OP;
	}

	return SHM_OK;
}


int read_shm_queue(void*handle, char *to_read_buf, int buf_len)
{
	int ret;
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;

	if(NULL == shmq_handle)
	{
		return SHM_HANDLE_NULL;
	}
	
	if(shmq_handle->op_mode != OP_MODE_READ)
	{
		return SHM_OP_WRONG;
	}

	ret = is_empty(shmq_handle->que);
	if(ret > 0)
	{
		return SHM_NO_DATA;
	}

	block_info_t *to_read_block;
	int off_set = sizeof(shm_queue_info_t) + sizeof(circular_array_t) + sizeof(block_info_t) * shmq_handle->op_pos;
	to_read_block = (block_info_t*)((char*)(shmq_handle->shm_que_info) + off_set);

	if(shmq_handle->op_pos == shmq_handle->que->tail)
	{
		return SHM_NO_DATA;
	}

	ret = get_read_num(shmq_handle->self_id);
	
	if(to_read_block->flags[ret] == FLAG_TRUE)
	{
		return SHM_DATA_READED_YET;
	}
	
	if(to_read_block->block_size <= 0)
	{
		return SHM_NO_DATA;
	}
	
	if(to_read_block->block_size > buf_len)
	{
		return SHM_BUF_SHORT;
	}
	
	int read_len = read_block(to_read_block,shmq_handle,to_read_buf, buf_len);

	to_read_block->flags[ret] = FLAG_TRUE;
	
	shmq_handle->op_total += (to_read_block->block_size);
	
	shmq_handle->op_pos = (shmq_handle->op_pos + 1) % shmq_handle->para_queue_size;
	
	return read_len;
}



int write_shm_queue(void* handle, const char* to_write_buf, int buf_len)
{
	
	int ret;
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	if(NULL == shmq_handle)
	{
		return SHM_HANDLE_NULL;
	}
	
	if(shmq_handle->op_mode != OP_MODE_WRITE)
	{
		return SHM_OP_WRONG;
	}

	if(shmq_handle->pool_size < buf_len)
	{
		return SHM_BUF_SHORT;
	}
	
	pthread_rwlock_wrlock(&(shmq_handle->shm_que_info->rwlock));
	int f_ret;
	while((f_ret = is_full(shmq_handle->que,shmq_handle->para_queue_size)) > 0 
		|| shmq_handle->shm_que_info->pool_left_size < buf_len)
	{
		block_info_t *head_block;
		int head_offset = sizeof(shm_queue_info_t) + sizeof(circular_array_t) + sizeof(block_info_t) * (shmq_handle->que->head);
		head_block = (block_info_t*)((char*)(shmq_handle->shm_que_info) + head_offset);

		if(shmq_handle->who_prior == READ_PRIO)
		{
			int i = 0;
			for( i = 0; i < MAX_PROC_NUM; i++)
			{
				if(shmq_handle->shm_que_info->read_flags[i] == FLAG_TRUE)
				{
					while(head_block->flags[i] == FLAG_FALSE)
					{
						/*
							i think a clock will be here when i have more time
						*/
					}
				}
			}
		}

		else if(shmq_handle->who_prior == WRITE_PRIO)
		{
			int i = 0;
			for(i = 0; i < MAX_PROC_NUM; i++)
			{
				if(shmq_handle->shm_que_info->read_flags[i] == FLAG_TRUE && head_block->flags[i] == FLAG_FALSE)
				{
					shmq_handle->readers_miss[i].miss_bytes += head_block->block_size;
					shmq_handle->readers_miss[i].miss_block_num++;
				}				
			}			
		}		

		
		ret = out_queue(shmq_handle->que,shmq_handle->para_queue_size);
		if(ret == SHM_OK)
		{			
			shmq_handle->shm_que_info->pool_left_size +=  head_block->block_size;
		}
		
		else
		{
			pthread_rwlock_unlock(&(shmq_handle->shm_que_info->rwlock));
			return SHM_OUT_QUEUE_FAIL;
		}	
	}

	ret = is_full(shmq_handle->que,shmq_handle->para_queue_size);  
	
	if(ret < 0 && shmq_handle->shm_que_info->pool_left_size >= buf_len)
	{   
		write_block(shmq_handle,to_write_buf,buf_len);
	}      
	
	else
	{
		pthread_rwlock_unlock(&(shmq_handle->shm_que_info->rwlock));
		return SHM_NO_SPACE;
	}           
	
	pthread_rwlock_unlock(&(shmq_handle->shm_que_info->rwlock));
	return SHM_OK;
}


int close_shm_queue(void*handle)
{

	int ret;
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	if(shmq_handle == NULL)
	{
		return SHM_HANDLE_NULL;
	}

	if(shmq_handle->self_id != shmq_handle->shm_que_info->owner_id)
	{
		return SHM_NO_AUTHORITY;
	}
	
	if(shmq_handle->shm_que_info->op_num != 1)
	{
		sleep(5);
		/*
			give 5 sec is enough
		*/
	}

	shmctl(shmq_handle->shm_fd,IPC_RMID,NULL);
	
	ret = shmget(shmq_handle->shm_fd,0,0);
	
	if(!ret)
	{
		return SHM_OK;
	}
	
	else
	{	
		return SHM_DELETE_SHM_FAIL;
	}
}



int destroy_shm_queue_handle(void*handle)
{

	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	int ret = -1;
	if(shmq_handle->op_mode == OP_MODE_READ)
	{
		shmq_handle->shm_que_info->read_flags[get_read_num(shmq_handle->self_id)] = '0';
		
		pthread_mutex_lock(&shmq_handle->shm_que_info->mutex);
		shmq_handle->shm_que_info->op_num--;
		pthread_mutex_unlock(&shmq_handle->shm_que_info->mutex);
		
		shmdt((char*)shmq_handle->shm_que_info);
		ret = SHM_OK;
	}	
	else if(shmq_handle->op_mode == OP_MODE_WRITE)
	{	
		ret = close_shm_queue(handle);
	}      
	free(handle);
	return ret;
}




