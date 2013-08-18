#ifndef __H_SHM_INNER_FUNC_HEADER_
#define __H_SHM_INNER_FUNC_HEADER_

#include "shm_core.h"

int init_read_handle_info(shm_queue_handle_t *handle,key_t key_id , char* sh_mem);

int init_write_handle_info(shm_queue_handle_t *handle, key_t key_id, char *sh_mem);

int  shm_init_mutext(pthread_mutex_t * mutex);

int  shm_init_rwlock(pthread_rwlock_t *rwlock);

int  init_circular_info(char* sh_mem);

int  copy_info_from_handle(char *sh_mem, shm_queue_handle_t *shmq_handle);


int  init_shm_info(char *sh_mem, shm_queue_handle_t *shmq_handle);


int read_block(block_info_t *to_read_block, shm_queue_handle_t *shmq_handle, char *to_read_buf, int buf_len);


int create_new_block(shm_queue_handle_t *shmq_handle, int buf_len, int is_split);


int update_handle_info(shm_queue_handle_t *shmq_handle, int buf_len);


int write_block(shm_queue_handle_t *shmq_handle,const char *to_write_buf,int buf_len);



#endif
