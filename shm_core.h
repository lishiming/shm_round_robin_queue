/*##################################################################################*//*

 *#  @FILE: shm_core.h
 *#  DESCRIPTION : 
      this file creat the core data hierarchy 
 *#  Author:lishiming2007@gmail.com
*####################################################################################*/

#ifndef __H_SHM_CORE_HEADER
#define __H_SHM_CORE_HEADER

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_circular_array.h"


#define MAX_PROC_NUM 32 /* this num only contains 32 process to read the shm,you can change if you want*/

#define FLAG_TRUE '1'
#define FLAG_FALSE '0'

#define SHM_NO_SPLITS 0
#define SHM_SPLITS 1

#define OP_MODE_READ	0x01 /* value of SHMQ_PARAMETER_OP_MODE,set the op mode read*/

#define OP_MODE_WRITE	0x02 /* value of SHMQ_PARAMETER_OP_MODE,set the op mode write*/

#define READ_PRIO	0x01 /* value of SHMQ_PARAMETER_WHO_PRIO,
				this will assure all the data to be read before covered */

#define WRITE_PRIO	0x02 /* value of SHMQ_PARAMETER_WHO_PRIO,
				this will assure the write process write as fast as it can,it will not wait for reader*/


#define SHMQ_PARAMETER_POOL_SIZE	0x01 /* type to set shm's size*/
#define SHMQ_PARAMETER_BLOCK_NUM	0x02 /* type to set the num of queue'elements*/
#define SHMQ_PARAMETER_OP_MODE		0x03 /* type to set op mod*/
#define SHMQ_PARAMETER_WHO_PRIO		0x04 /* type to set read is prio or write is*/


enum SHM_RETURN_RET{	
			SHM_HANDLE_NULL = -201, /* error: the handle is null*/
			SHM_UNKNOWN_OP, /* error: unknown op type*/
			SHM_OP_WRONG, /* error: op value can't map the op mod*/
			SHM_UNKNOWN_PRIO, /* error:the value of SHMQ_PARAMETER_WHO_PRIO is wrong*/
			SHM_NO_AUTHORITY, /* error:the process don't have been authorized*/
			SHM_UNKNOWN_TYPE, /* error:set a unknown type*/
			SHM_NO_SHM, /* error:the share memory is not exist*/
			SHM_CREATE_SHMEM_FAIL, /* error: fail to create share memory*/
			SHM_ALREADY_EXIST, /* error: the share memory is exist,can't create it repeatedly*/
			SHM_NO_DATA, /* error: no data in the share memory*/
			SHM_DATA_READED_YET, /* error: this data block has been read by the same process*/
			SHM_OUT_QUEUE_FAIL, /* error: fail to dequeue the tail*/
			SHM_NO_SPACE, /* error: no space left to write data to share memory*/
			SHM_DELETE_SHM_FAIL, /* error:delete the share memory failed*/
			SHM_FAIL = -1, /* error:the function gets a fail result*/
			SHM_OK = 1 /* the function success*/
		  };


/**
 * 
 * STRUCT NAME: shm_queue_info_t
 * DESCRIPTION: this struct contains the basic info of share memory  
 */
 
typedef struct _shm_queue_info
{
	unsigned int max_queue_size; /* the max elements's num of the queue*/
	unsigned int pool_size; /* size of the memory pool(data block will be here)*/
	unsigned int pool_left_size; /* size of the memory pool can be used*/
    	unsigned int pool_begin_offset; /* the offset of the memory pool's start to the start of the whole share memory*/
    	unsigned int pool_end_offset; /*the offset of the memory pool's end to the start of the whole share memory*/
    	unsigned int write_pos; /* the offset of the next write position to the start of the whole share memory*/
	unsigned int owner_id; /* which process create the share memory(not pid)*/
	unsigned int op_num; /* the num of processes who attached to the share memory*/
	char read_flags[MAX_PROC_NUM]; /* this array marks which reader is reading share memory*/
	pthread_mutex_t mutex; /* mutext protect op_num*/
    	pthread_rwlock_t rwlock; /* writer should get rwlock when write,reader only get trwlock when he first get head of queue*/
       
}shm_queue_info_t;


/**
 * 
 * STRUCT NAME: miss_reader_stat_t
 * DESCRIPTION: when SHMQ_PARAMETER_WHO_PRIO is WRITE_PRIO ,this struct record the info of reader missed
 */
 
typedef struct _miss_reader_stat_t
{
	unsigned long long	miss_block_num; /* the num of missed block*/
	unsigned long long	miss_bytes; /* the bytes of missed data*/

}miss_reader_stat_t;



/**
 * 
 * STRUCT NAME: shm_queue_handle_t
 * DESCRIPTION: process must have handle before read or write
 */
typedef struct _shm_queue_handle_t
{       
        
	unsigned int	self_id; /* id contributed to process,not pid*/
	unsigned int	op_mode; /* read or write*/
	unsigned int	op_pos; /* offset the process has read or write */
	unsigned int	who_prior; /*SHMQ_PARAMETER_WHO_PRIO*/
	unsigned long long	op_total; /* the betys process has read or written*/
	key_t   key_id; /* key_id is the id of share memory*/
	int	shm_fd; /* mid of share memory*/
	unsigned int	para_queue_size; /* the queue elements' num*/
	unsigned int	para_pool_size; /* size of pool to store data*/
	miss_reader_stat_t	readers_miss[MAX_PROC_NUM]; /* if WRITE_PRIO,how many betys the readers have missed*/	
	shm_queue_info_t	*shm_que_info; /* start of share memry*/
	circular_array_t	*que; /* start of array*/

}shm_queue_handle_t;


#endif
