/*####################################################################################*//**

 *#  @FILE: shm_check_authorize.h
 *#  DESCRIPTION : 
      this file is used for checking the process's authorize
      if the process don't have a right id, the interface
      will refuse the read or write operation.
 *#  Author:lishiming2007@gmail.com
*####################################################################################*/
#ifndef __H_CHECK_AUTHORIZE_HEADER_
#define __H_CHECK_AUTHORIZE_HEADER_

#ifndef SHM_TRUE
#define SHM_TRUE 1
#endif

#ifndef SHM_FAIL
#define SHM_FALSE -1
#endif


 /**
 * 
 * METHOD: check_read_id
 * DESCRIPTION: check the id whether has been authorized  
 * @param :id, the read id who is distributed to the read process,not pid!
 * @return :SHM_TRUE if this id has been authorized,if not,SHM_FALSE is returned
 */

int check_read_id(int id);


/**
 * 
 * METHOD: get_read_num
 * DESCRIPTION: actually，the id can be mapped to a num between 1-32,we can use this num to stand for the process
 * @param :id,the read id who is distributed to the read process,not pid!
 * @return :the num between 1-32
 */

int get_read_num(int id);


/**
 * 
 * METHOD: check_write_id
 * DESCRIPTION: check the id whether has been authorized  
 * @param :id, the write id who is distributed to the write process,not pid!
 * @return :SHM_TRUE if this id has been authorized,if not,SHM_FALSE is returned
 */
 
int check_write_id(int id);

/**
 * 
 * METHOD: get_write_num
 * DESCRIPTION: actually，the id can be mapped to a num between 1-32,we can use this num to stand for the process
 * @param :id,the write id who is distributed to the write process,not pid!
 * @return :the num between 1-32
 */
 
int get_write_num(int id);

#endif
