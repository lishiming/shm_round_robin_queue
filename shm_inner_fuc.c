#include "shm_inner_fuc.h"
#include <string.h>

int init_read_handle_info(shm_queue_handle_t *handle,key_t key_id , char *sh_mem)
{
	if(NULL == handle || NULL == sh_mem)
	{
		return SHM_FAIL;
	}
	handle->shm_fd = shmget(key_id,0,0);
	
	if(handle->shm_fd < 0)
	{
		return SHM_FAIL;
	}

	//sh_mem = (char*)shmat(handle->shm_fd,NULL,0);
	
	handle->shm_que_info = (shm_queue_info_t*)sh_mem;
	handle->que = (circular_array_t*)((char*)sh_mem + sizeof(shm_queue_info_t));
	handle->para_queue_size = handle->shm_que_info->max_queue_size;
	handle->para_pool_size =  handle->shm_que_info->pool_size;
			
	pthread_rwlock_rdlock(&handle->shm_que_info->rwlock);			
	handle->op_pos =  handle->que->head;
	pthread_rwlock_unlock(&handle->shm_que_info->rwlock);	
	handle->shm_que_info->read_flags[get_read_num(handle->self_id)] = FLAG_TRUE;
	handle->op_total = 0;
	
	pthread_mutex_lock(&handle->shm_que_info->mutex);
	handle->shm_que_info->op_num++;
	pthread_mutex_unlock(&handle->shm_que_info->mutex);

	return SHM_OK;
} 



int init_write_handle_info(shm_queue_handle_t *shmq_handle, key_t key_id, char *sh_mem)
{
	if(NULL == shmq_handle || NULL == sh_mem)
	{
		return SHM_FAIL;
	}
	shmq_handle->shm_fd = shmget(key_id, 0, 0);

	if(shmq_handle->shm_fd < 0)
	{
		return SHM_FAIL;
	}

	//shmq_handle->shm_que_info = (shm_queue_info_t*)shmat(shmq_handle->shm_fd, NULL, 0);
	shmq_handle->shm_que_info = (shm_queue_info_t*)sh_mem;
	shmq_handle->key_id = key_id;
	shmq_handle->que = (circular_array_t*)((char*)shmq_handle->shm_que_info + sizeof(shm_queue_info_t));
	shmq_handle->op_total = 0;
	
	pthread_mutex_lock(&shmq_handle->shm_que_info->mutex);
	shmq_handle->shm_que_info->op_num = 1;
	pthread_mutex_unlock(&shmq_handle->shm_que_info->mutex);
	
	int i = 0;
	for(i = 0; i < MAX_PROC_NUM; i++)
	{
		shmq_handle->readers_miss[i].miss_bytes = 0;
		shmq_handle->readers_miss[i].miss_block_num = 0;
	}

	return SHM_OK;
}


int  shm_init_mutext(pthread_mutex_t * mutex)
{
	int ret = -1;
	pthread_mutexattr_t mattr;     
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
	ret = pthread_mutex_init(mutex, &mattr);
	return ret;
}


int  shm_init_rwlock(pthread_rwlock_t *rwlock)
{	
	int ret = -1;
	pthread_rwlockattr_t attr;
	pthread_rwlockattr_init(&attr);
	pthread_rwlockattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	ret = pthread_rwlock_init(rwlock, &attr);
	return ret;
}


int  init_circular_info(char* sh_mem)
{
	if(NULL == sh_mem)
	{
		return SHM_FAIL;
	}
	circular_array_t *circular_array  = (circular_array_t *)((char*)sh_mem + sizeof(shm_queue_info_t) );
	circular_array->pbase = (block_info_t*)((char*)sh_mem + sizeof(shm_queue_info_t)+ sizeof(circular_array_t));
	circular_array->head = 0;
	circular_array->tail = 0;
	return SHM_OK;
}


int  copy_info_from_handle(char *sh_mem, shm_queue_handle_t *shmq_handle)
{
	if(NULL == sh_mem || NULL == shmq_handle)
	{
		return SHM_FAIL;
	}

	shm_queue_info_t *shmem_info = (shm_queue_info_t *)sh_mem;
	shmem_info->owner_id = shmq_handle->self_id;
	shmem_info->op_num = 1;
	shmem_info->max_queue_size = shmq_handle->para_queue_size;
	shmem_info->pool_size = shmq_handle->para_pool_size;
	shmem_info->pool_left_size = shmq_handle->para_pool_size;
	shmem_info->pool_begin_offset = sizeof(shm_queue_info_t) + sizeof(circular_array_t) + sizeof(block_info_t) * shmem_info->max_queue_size;
    	
	shmem_info->pool_end_offset = shmem_info->pool_begin_offset + shmem_info->pool_size - 1;
    	shmem_info->write_pos = shmem_info->pool_begin_offset;

	return SHM_OK;
}

int  init_shm_info(char *sh_mem, shm_queue_handle_t *shmq_handle)
{	
	int mem_len = sizeof(shm_queue_info_t) + 
			sizeof(circular_array_t) + 
			shmq_handle->para_pool_size +
			shmq_handle->para_queue_size * sizeof(block_info_t);
	
	memset(sh_mem, 0, mem_len);
	
	copy_info_from_handle(sh_mem, shmq_handle);
	init_circular_info(sh_mem);    	
	int j;
	for(j = 0;j < MAX_PROC_NUM; j++)
	{
		((shm_queue_info_t*)sh_mem) -> read_flags[j] = FLAG_FALSE;
	}
	int ret1 = shm_init_mutext((pthread_mutex_t *)(&(((shm_queue_info_t *)sh_mem)->rwlock)));
	int ret2 = shm_init_rwlock((pthread_rwlock_t *)(&(((shm_queue_info_t *)sh_mem)->rwlock)));

	if(0 == ret1 && 0 == ret2)
	{
		return SHM_OK;
	}
	else
	{
		return SHM_FAIL;
	}
}


int read_block(block_info_t *to_read_block,shm_queue_handle_t *shmq_handle,char *to_read_buf, int buf_len)
{
	if(to_read_block->block_size > buf_len)
	{
		return -1;
	}
	
	int begin_offset = shmq_handle->shm_que_info->pool_begin_offset;
	int end_offset = shmq_handle->shm_que_info->pool_end_offset;
	int w_pos = shmq_handle->shm_que_info->write_pos;
	
	if(0 == to_read_block->is_splits)
	{
		char * read_start_addr = (char*)shmq_handle->shm_que_info + to_read_block->block_start;        
		memcpy(to_read_buf,read_start_addr,to_read_block->block_size);
	}
	else
	{
		char *first_read_start_addr =  (char*)shmq_handle->shm_que_info + to_read_block->block_start;
		int first_read_size = end_offset - to_read_block->block_start + 1;
		memcpy(to_read_buf,first_read_start_addr,first_read_size);
		
		char * second_to_read_buf = to_read_buf + end_offset - to_read_block->block_start + 1;		
		
		char * second_read_start_addr = (char*)shmq_handle->shm_que_info + begin_offset;
		
		int second_read_size = to_read_block->block_size - (end_offset - to_read_block->block_start + 1);	
		memcpy(second_to_read_buf,second_read_start_addr,second_read_size);
    	}
	
	return to_read_block->block_size;
}


int create_new_block(shm_queue_handle_t *shmq_handle, int buf_len, int is_split)
{
	if(NULL == shmq_handle)
	{
		return SHM_FAIL;
	}
	
	int begin_offset = shmq_handle->shm_que_info->pool_begin_offset;
	int end_offset = shmq_handle->shm_que_info->pool_end_offset;
	int w_pos = shmq_handle->shm_que_info->write_pos;
	int off_len = sizeof(shm_queue_info_t) + sizeof(circular_array_t) + sizeof(block_info_t) * shmq_handle->que->tail ;
	
	block_info_t *block_info = (block_info_t*)((char*)(shmq_handle->shm_que_info) + off_len);

	block_info->block_size = buf_len;
	block_info->block_start = w_pos;		
	if(is_split == SHM_NO_SPLITS)
	{
		block_info->block_end = (block_info->block_start + buf_len - 1);
	}
	else if(is_split == SHM_SPLITS)
	{
		block_info->block_end = begin_offset + buf_len - (end_offset - w_pos + 1) - 1;
	}
	block_info->is_splits = is_split;
	
	int j;
	for(j = 0;j < MAX_PROC_NUM; j++)
	{
		block_info->flags[j] = FLAG_FALSE;
	}

	return SHM_OK;
	
}

int update_handle_info(shm_queue_handle_t *shmq_handle, int buf_len)
{
	if(NULL == shmq_handle || buf_len <= 0)
	{
		return SHM_FAIL;
	}

	int begin_offset = shmq_handle->shm_que_info->pool_begin_offset;
	int end_offset = shmq_handle->shm_que_info->pool_end_offset;
	int w_pos = shmq_handle->shm_que_info->write_pos;

	shmq_handle->que->tail = (shmq_handle->que->tail + 1) % shmq_handle->para_queue_size;
	shmq_handle->shm_que_info->pool_left_size -= buf_len;
		
	if(end_offset - w_pos + 1 > buf_len)
	{
		shmq_handle->shm_que_info->write_pos += buf_len;
	}
	else if(end_offset - w_pos + 1 == buf_len)
	{
		shmq_handle->shm_que_info->write_pos = begin_offset;
	}
	else
	{
		shmq_handle->shm_que_info->write_pos = begin_offset + buf_len - (end_offset - w_pos + 1);
	}
	shmq_handle->op_total += buf_len;

	return SHM_OK;
}


int write_block(shm_queue_handle_t *shmq_handle,const char *to_write_buf,int buf_len)
{
	if(NULL == shmq_handle || NULL == to_write_buf || buf_len <= 0)
	{
		return SHM_FAIL;
	}
	
	int begin_offset = shmq_handle->shm_que_info->pool_begin_offset;
	int end_offset = shmq_handle->shm_que_info->pool_end_offset;
	int w_pos = shmq_handle->shm_que_info->write_pos;
	
	if(end_offset - w_pos + 1 >= buf_len)
	{
		char * to_write_addr = (char*)shmq_handle->shm_que_info + w_pos;
		memcpy(to_write_addr,to_write_buf,buf_len);
		
		create_new_block(shmq_handle, buf_len, SHM_NO_SPLITS);
		update_handle_info(shmq_handle, buf_len);
	}
		
        else if(end_offset - w_pos + 1 < buf_len)
	{
		char * first_cpy_addr = (char*)shmq_handle->shm_que_info + w_pos;
		int first_cpy_size = end_offset - w_pos +1;		
		memcpy(first_cpy_addr,to_write_buf,first_cpy_size);
		
		char * second_cpy_addr = (char*)shmq_handle->shm_que_info + begin_offset;
		char * second_buf_addr = (char*)to_write_buf + end_offset - w_pos + 1;
		int second_cpy_size =  buf_len - (end_offset - w_pos + 1);
		memcpy(second_cpy_addr,second_buf_addr,second_cpy_size );
                   
		create_new_block(shmq_handle, buf_len, SHM_SPLITS);
		update_handle_info(shmq_handle, buf_len);
	}
	return SHM_OK;
}
