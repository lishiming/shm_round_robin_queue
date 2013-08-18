#include "shm_check_authorize.h"

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

int get_read_num(int id)
{
	int num = ((id & 0xffff0000) >>(32 - 16)) | ((id & 0xffff0000) << 16);
	return num;
}


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


int get_write_num(int id)
{
	int num = ((id & 0xffff0000) >>(32 - 16)) | ((id & 0xffff0000) << 16);
	return num;
}

