#include "shm_check_authorize.h"
/****************************************************************
	this file is used for checking the process's authorize
	if the process don't have a right id, the interface
	will refuse the read or write operation.
 ***************************************************************/



/**
 * 
 * METHOD: check_read_id
 * DESCRIPTION: check the id whether has been authorized  
 * @param :id, the read id who is distributed to the read process,not pid!
 * @return :SHM_TRUE if this id has been authorized,if not,SHM_FALSE is returned
 */

int check_read_id(int id)
{
	int m = id & 0x0000ffff ^ 0x0000f5bf ^ 0x00002220;
	if(1 == m)
	{
		return SHM_TRUE;
	}
	
	else
	{	
		return SHM_FALSE;
	}
}


/**
 * METHOD: get_read_num
 * DESCRIPTION: actually，the id can be mapped to a num between 1-32,we can use this num to stand for the process
 * @param :id,the read id who is distributed to the read process,not pid!
 * @return :the num between 1-32
 */
 
int get_read_num(int id)
{
	int num = ((id & 0xffff0000) >>(32 - 16)) | ((id & 0xffff0000) << 16);
	return num;
}



/**
 * 
 * METHOD: check_write_id
 * DESCRIPTION: check the id whether has been authorized  
 * @param :id, the write id who is distributed to the write process,not pid!
 * @return :SHM_TRUE if this id has been authorized,if not,SHM_FALSE is returned
 */
 
int check_write_id(int id)
{
	int m= id & 0x0000ffff ^ 0x0000814a ^ 0x0000f4e0;
	if(1 == m)
	{
		return SHM_TRUE;
	}
	else
	{
		return SHM_FALSE;
	}
}

/**
 * METHOD: get_write_num
 * DESCRIPTION: actually，the id can be mapped to a num between 1-32,we can use this num to stand for the process
 * @param :id,the write id who is distributed to the write process,not pid!
 * @return :the num between 1-32
 */
 
int get_write_num(int id)
{
	int num = ((id & 0xffff0000) >>(32 - 16)) | ((id & 0xffff0000) << 16);
	return num;
}

