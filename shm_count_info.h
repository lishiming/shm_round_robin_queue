#ifndef __H_SHM_COUNT_INFO_HEADER_
#define __H_SHM_COUNT_INFO_HEADER_


extern int check_read_id(int id);


unsigned long long  get_op_total(void*handle);


int get_miss_stat(void*handle, int id, unsigned long long *miss_block, unsigned long long *miss_betys);

#endif
