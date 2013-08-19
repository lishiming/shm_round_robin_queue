#ifndef __H_SHM_COUNT_INFO_HEADER_
#define __H_SHM_COUNT_INFO_HEADER_


extern int check_read_id(int id);

/**
 * 
 * METHOD: get_op_total
 * DESCRIPTION: get the  total data process has read or writen  
 * @param : handle,the handle created
 * @return : how many bytes process has read or writen
 */
 
unsigned long long  get_op_total(void*handle);


/**
 * 
 * METHOD: get_miss_stat
 * DESCRIPTION: this function is used by write processto get a read process miss info 
 * @param : handle,the handle created
 * @param : id, read process's id(not pid)
 * @param : miss_block, output how many blocks the reader has missed
 * @param : miss_bytes, output how many bytes the reader has missed
 * @return : SHM_OK if success,SHM_FAIL if fail
 */
 
int get_miss_stat(void*handle, int id, unsigned long long *miss_block, unsigned long long *miss_bytes);

#endif
