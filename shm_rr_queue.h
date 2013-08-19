#ifndef __H_SHM_RR_QUEUE_HEADER_
#define __H_SHM_RR_QUEUE_HEADER_

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
 * @param : type, SHMQ_PARAMETER_POOL_SIZE,SHMQ_PARAMETER_BLOCK_NUM,SHMQ_PARAMETER_OP_MODE,SHMQ_PARAMETER_WHO_PRIO in
 *          file shm_core.h
 * @param : value, in shm_core.h
 * @return : value in shm_core.h enum SHM_FAIL_RET
 */
 
int set_shm_queue_handle(void *handle, int type, int value);


/**
 * 
 * METHOD: open_shm_queue
 * DESCRIPTION: open the shared memory, write process will create shared memory 
 * @param : handle,can not NULL
 * @param : key_id, shared memory's key_id
 * @return : value in shm_core.h enum SHM_FAIL_RET
 */
 
int open_shm_queue(void*handle, key_t key_id);


/**
 * 
 * METHOD: read_shm_queue
 * DESCRIPTION: read a block from shared memory
 * @param : handle,can not NULL
 * @param : to_read_buf, buf 
 * @param : buf_len, length of buf
 * @return : length read from shared memory,if fail return value in shm_core.h enum SHM_FAIL_RET
 */
 
int read_shm_queue(void*handle, char *to_read_buf, int buf_len);


/**
 * 
 * METHOD: write_shm_queue
 * DESCRIPTION: write a block to shared memory
 * @param : handle,can not NULL
 * @param : to_write_buf, buf 
 * @param : buf_len, length of buf
 * @return :  return value in shm_core.h enum SHM_FAIL_RET
 */
 
int write_shm_queue(void* handle, const char* to_write_buf, int buf_len);


/**
 * 
 * METHOD: close_shm_queue
 * DESCRIPTION: writer process close shared memory 
 * @param : handle,can not NULL
 * @return :  return value in shm_core.h enum SHM_FAIL_RET
 */
 
int close_shm_queue(void*handle);


/**
 * 
 * METHOD: destroy_shm_queue_handle
 * DESCRIPTION: destroy handle
 * @param : handle,can not NULL
 * @return :  return value in shm_core.h enum SHM_FAIL_RET
 */
 
int destroy_shm_queue_handle(void*handle);


#endif
