#ifndef __H_SNOW_BALL_HEADER
#define __H_SNOW_BALL_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OP_MODE_READ
#define OP_MODE_READ	0x01 /* value of SHMQ_PARAMETER_OP_MODE,set the op mode read*/
#endif

#ifndef OP_MODE_WRITE
#define OP_MODE_WRITE	0x02 /* value of SHMQ_PARAMETER_OP_MODE,set the op mode write*/
#endif

#ifndef READ_PRIO
#define READ_PRIO	0x01 /* value of SHMQ_PARAMETER_WHO_PRIO,
				                this will assure all the data to be read before covered */
#endif

#ifndef WRITE_PRIO
#define WRITE_PRIO	0x02 /* value of SHMQ_PARAMETER_WHO_PRIO,
				                  this will assure the write process write as fast as it can,it will not wait for reader*/
#endif

#ifndef SHMQ_PARAMETER_POOL_SIZE
#define SHMQ_PARAMETER_POOL_SIZE	0x01 /* type to set shm's size*/
#endif

#ifndef SHMQ_PARAMETER_BLOCK_NUM
#define SHMQ_PARAMETER_BLOCK_NUM	0x02 /* type to set the num of queue'elements*/
#endif

#ifndef SHMQ_PARAMETER_OP_MODE
#define SHMQ_PARAMETER_OP_MODE		0x03 /* type to set op mod*/
#endif

#ifndef SHMQ_PARAMETER_WHO_PRIO
#define SHMQ_PARAMETER_WHO_PRIO		0x04 /* type to set read is prio or write is*/
#endif

#ifndef RETURN_RET
#define RETURN_RET 1

enum SHM_RETURN_RET
{	
			SHM_HANDLE_NULL = -201, /* error: the handle is null*/
			SHM_UNKNOWN_OP, /* error: unknown op type*/
			SHM_OP_WRONG, /* error: op value can't map the op mod*/
			SHM_UNKNOWN_PRIO, /* error:the value of SHMQ_PARAMETER_WHO_PRIO is wrong*/
			SHM_NO_AUTHORITY, /* error:the process don't have been authorized*/
			SHM_UNKNOWN_TYPE, /* error:set a unknown type*/
			SHM_NO_SHM, /* error:the shared memory is not exist*/
			SHM_CREATE_SHMEM_FAIL, /* error: fail to create shared memory*/
			SHM_ALREADY_EXIST, /* error: the shared memory is exist,can't create it repeatedly*/
			SHM_NO_DATA, /* error: no data in the shared memory*/
			SHM_DATA_READED_YET, /* error: this data block has been read by the same process*/
			SHM_OUT_QUEUE_FAIL, /* error: fail to dequeue the tail*/
			SHM_NO_SPACE, /* error: no space left to write data to shared memory*/
			SHM_DELETE_SHM_FAIL, /* error:delete the shared memory failed*/
			SHM_BUF_SHORT,/*error: when read if buf is too short,when write if shm is too short*/
			SHM_FAIL = -1, /* error:the function gets a fail result*/
			SHM_OK = 1 /* the function success*/
};
#endif

/**
 * 
 * METHOD: get_shm_queue_handle
 * DESCRIPTION: get a handle  
 * @param : self_id , a id contributed to process 
 * @return : NULL if fail, not NULL if success
 */
 
void *get_shm_queue_handle(int self_id);

/**
 * 
 * METHOD: set_shm_queue_handle
 * DESCRIPTION: set shared memory's info 
 * @param : handle, handle not NULL
 * @param : type, SHMQ_PARAMETER_POOL_SIZE,SHMQ_PARAMETER_BLOCK_NUM,SHMQ_PARAMETER_OP_MODE,SHMQ_PARAMETER_WHO_PRIO
 * @param : value,
 * @return : value in  enum SHM_FAIL_RET
 */
 
int set_shm_queue_handle(void *handle, int type, int value);

/**
 * 
 * METHOD: open_shm_queue
 * DESCRIPTION: open the shared memory, write process will create shared memory 
 * @param : handle,can not NULL
 * @param : key_id, shared memory's key_id
 * @return : value in  enum SHM_FAIL_RET
 */
 
int open_shm_queue(void*handle, key_t key_id);

/**
 * 
 * METHOD: read_shm_queue
 * DESCRIPTION: read a block from shared memory
 * @param : handle,can not NULL
 * @param : to_read_buf, buf 
 * @param : buf_len, length of buf
 * @return : length read from shared memory,if fail return value in enum SHM_FAIL_RET
 */
 
int read_shm_queue(void*handle, char *to_read_buf, int buf_len);

/**
 * 
 * METHOD: write_shm_queue
 * DESCRIPTION: write a block to shared memory
 * @param : handle,can not NULL
 * @param : to_write_buf, buf 
 * @param : buf_len, length of buf
 * @return :  return value in enum SHM_FAIL_RET
 */
 
int write_shm_queue(void* handle, const char* to_write_buf, int buf_len);


/**
 * 
 * METHOD: destroy_shm_queue_handle
 * DESCRIPTION: destroy handle
 * @param : handle,can not NULL
 * @return :  return value in enum SHM_FAIL_RET
 */
 
int destroy_shm_queue_handle(void*handle);


#ifdef __cplusplus
}
#endif

#endif
