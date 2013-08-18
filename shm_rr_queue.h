#ifndef __H_SHM_RR_QUEUE_HEADER_
#define __H_SHM_RR_QUEUE_HEADER_


void *get_shm_queue_handle(int self_id);


int set_shm_queue_handle(void *handle, int type, int value);


int open_shm_queue(void*handle, key_t key_id);


int read_shm_queue(void*handle, char *to_read_buf, int buf_len);


int write_shm_queue(void* handle, const char* to_write_buf, int buf_len);


int close_shm_queue(void*handle);


int destroy_shm_queue_handle(void*handle);


#endif
