#ifndef __H_CHECK_AUTHORIZE_HEADER_
#define __H_CHECK_AUTHORIZE_HEADER_

#ifndef SHM_TRUE
#define SHM_TRUE 1
#endif

#ifndef SHM_FAIL
#define SHM_FALSE -1
#endif

int check_read_id(int id);

int get_read_num(int id);

int check_write_id(int id);

int get_write_num(int id);

#endif
