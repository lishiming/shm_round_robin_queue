#ifndef __H_SHM_CORE_HEADER
#define __H_SHM_CORE_HEADER

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_circular_array.h"


#define MAX_PROC_NUM 32

#define FLAG_TRUE '1'
#define FLAG_FALSE '0'

#define SHM_NO_SPLITS 0
#define SHM_SPLITS 1

#define OP_MODE_READ	0x01
#define OP_MODE_WRITE	0x02
#define READ_PRIO	0x01
#define WRITE_PRIO	0x02

#define SHMQ_PARAMETER_POOL_SIZE	0x01
#define SHMQ_PARAMETER_BLOCK_NUM	0x02
#define SHMQ_PARAMETER_OP_MODE		0x03
#define SHMQ_PARAMETER_WHO_PRIO		0x04

enum SHM_RETURN_RET{	
			SHM_HANDLE_NULL = -201,
			SHM_UNKNOWN_OP,
			SHM_OP_WRONG,
			SHM_UNKNOWN_PRIO,
			SHM_NO_AUTHORITY,
			SHM_UNKNOWN_TYPE,
			SHM_NO_SHM,
			SHM_CREATE_SHMEM_FAIL,
			SHM_ALREADY_EXIST,
			SHM_NO_DATA,
			SHM_DATA_READED_YET,
			SHM_OUT_QUEUE_FAIL,
			SHM_NO_SPACE,
			SHM_DELETE_SHM_FAIL,
			SHM_FAIL = -1,
			SHM_OK = 1
		  };


typedef struct _shm_queue_info
{
	unsigned int max_queue_size;
	unsigned int pool_size;
	unsigned int pool_left_size;
    	unsigned int pool_begin_offset;
    	unsigned int pool_end_offset;
    	unsigned int write_pos;
	unsigned int owner_id;
	unsigned int op_num;
	char read_flags[MAX_PROC_NUM];
	pthread_mutex_t mutex;
    	pthread_rwlock_t rwlock;
       
}shm_queue_info_t;


typedef struct _miss_reader_stat_t
{
	unsigned long long	miss_block_num;
	unsigned long long	miss_bytes;

}miss_reader_stat_t;


typedef struct _shm_queue_handle_t
{       
        
	unsigned int	self_id;
	unsigned int	op_mode;
	unsigned int	op_pos;
	unsigned int	who_prior;
	unsigned long long	op_total;
	key_t   key_id;
	int	shm_fd;
	unsigned int	para_queue_size;
	unsigned int	para_pool_size;
	miss_reader_stat_t	readers_miss[MAX_PROC_NUM];	
	shm_queue_info_t	*shm_que_info;
	circular_array_t	*que;

}shm_queue_handle_t;


#endif
