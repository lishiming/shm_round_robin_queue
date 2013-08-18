#include "shm_circular_array.h"
#include <stdio.h>

int  init_queue(circular_array_t *pqueue, block_info_t *pbase)
{
	if(NULL == pqueue || NULL == pbase)
	{
		return SHM_FALSE;
	}
	
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->pbase = pbase;

	return SHM_TRUE;
}


int is_full(circular_array_t *pqueue,unsigned int queue_size)
{
	if((pqueue->tail + 1) % queue_size == pqueue->head)
	{
		return SHM_TRUE;
	}
	else
	{
		return SHM_FALSE;
	}
}


int is_empty(circular_array_t *pqueue)
{
	if(pqueue->head == pqueue->tail)
	{
		return SHM_TRUE;
	}
	else
	{
		return SHM_FALSE;
	}
}


int out_queue(circular_array_t *pqueue,unsigned int queue_size)
{
	int ret = is_empty(pqueue);
	if(ret == SHM_TRUE)
	{
		return SHM_FALSE;
	}
	else
	{
		pqueue->head = (pqueue->head + 1) % queue_size;
		return SHM_TRUE;
	}
}


int en_queue(circular_array_t *pqueue,block_info_t *value,unsigned int queue_size)
{
	if(is_full(pqueue,queue_size))
	{
		return SHM_FALSE;
	}
	else
	{
		pqueue->pbase[pqueue->tail] = *value;
		pqueue->tail = (pqueue->tail + 1) % queue_size;
		return SHM_TRUE;
	}
}

