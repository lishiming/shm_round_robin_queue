#include "shm_count_info.h"
#include "shm_core.h"


unsigned long long  get_op_total(void*handle)
{	
	if(NULL == handle)
	{
		return SHM_FAIL;
	}
	
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	return shmq_handle->op_total;
}



int  get_miss_stat(void*handle, int id, unsigned long long *miss_block, unsigned long long *miss_bytes)
{
	shm_queue_handle_t *shmq_handle = (shm_queue_handle_t*)handle;
	
	if(NULL == shmq_handle)
	{
		return SHM_FAIL;
	}
	int i = get_read_num(id);

	*miss_block = shmq_handle->readers_miss[i].miss_block_num;
	*miss_betys = shmq_handle->readers_miss [i].miss_bytes;
	return SHM_OK;
}
