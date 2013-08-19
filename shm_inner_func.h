#ifndef __H_SHM_INNER_FUNC_HEADER_
#define __H_SHM_INNER_FUNC_HEADER_

#include "shm_core.h"


/**
 * 
 * METHOD: init_read_handle_info
 * DESCRIPTION: init read process's handle,copy shared memory's info to the handle
 * @param :key_id,shared memory's key_id
 * @param :sh_mem,the address of shared memory
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int init_read_handle_info(shm_queue_handle_t *handle,key_t key_id , char* sh_mem);


/**
 * 
 * METHOD: init_write_handle_info
 * DESCRIPTION: init write process's handle,copy shared memory's info to the handle
 * @param :key_id,shared memory's key_id
 * @param :sh_mem,the address of shared memory
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
 
int init_write_handle_info(shm_queue_handle_t *handle, key_t key_id, char *sh_mem);


/**
 * 
 * METHOD: shm_init_mutext
 * DESCRIPTION: init a mutex
 * @param :mutex,pointer of mutex
 * @return :return the result of  pthread_mutex_init
 */
 
int  shm_init_mutext(pthread_mutex_t * mutex);

/**
 * 
 * METHOD: shm_init_rwlock
 * DESCRIPTION: init a rwlock
 * @param :rwlock,pointer of rwlock
 * @return :return the result of  pthread_rwlock_init
 */
 
int  shm_init_rwlock(pthread_rwlock_t *rwlock);


/**
 * 
 * METHOD: init_circular_info
 * DESCRIPTION: init circular_array_t struct:head,tail,pbase
 * @param :sh_mem, the address of shared memory
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int  init_circular_info(char* sh_mem);


/**
 * 
 * METHOD: copy_info_from_handle
 * DESCRIPTION: copy info (owner_id,max_queue_size and so on)
 * @param :sh_mem, the address of shared memory
 * @param :shmq_handle,
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int  copy_info_from_handle(char *sh_mem, shm_queue_handle_t *shmq_handle);


/**
 * 
 * METHOD: init_shm_info
 * DESCRIPTION: init shared memory info
 * @param :sh_mem, the address of shared memory
 * @param :shmq_handle,
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int init_shm_info(char *sh_mem, shm_queue_handle_t *shmq_handle);


/**
 * 
 * METHOD: read_block
 * DESCRIPTION: read a block of data from shared memory to process's buf
 * @param : to_read_block,block info of data to be read
 * @param : shmq_handle,handle
 * @param : to_read_buf,the buf to store data
 * @param : buf_len, length of buf
 * @return : SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int read_block(block_info_t *to_read_block, shm_queue_handle_t *shmq_handle, char *to_read_buf, int buf_len);


/**
 * 
 * METHOD: create_new_block
 * DESCRIPTION: create a new block info to the queue
 * @param :shmq_handle,
 * @param : buf_len,length of data
 * @param : is_split,SHM_NO_SPLITS or SHM_SPLITS
 * @return :SHM_TRUE if success,if not,SHM_FALSE is returned
 */
 
int create_new_block(shm_queue_handle_t *shmq_handle, int buf_len, int is_split);


/**
 * 
 * METHOD: update_handle_info
 * DESCRIPTION: update handle's info after read or write
 * @param :shmq_handle,
 * @param : buf_len,length of data
 * @return :SHM_OK if success,if not,SHM_FAIL is returned
 */
 
int update_handle_info(shm_queue_handle_t *shmq_handle, int buf_len);


/**
 * 
 * METHOD: write_block
 * DESCRIPTION: write a block of data from shared memory from process's buf
 * @param : shmq_handle,handle
 * @param : to_write_buf ,pointer of buf
 * @param : buf_len, length of buf
 * @return :SHM_OK if success,if not,SHM_FAIL is returned
 */
 
int write_block(shm_queue_handle_t *shmq_handle,const char *to_write_buf,int buf_len);



#endif
