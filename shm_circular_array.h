/*##################################################################################*//*

 *#  @FILE: shm_circular_array.h
 *#  DESCRIPTION : 
      in this file,I use an array to realize a round-robin queue, I don't use the link list
      because the round-robin queue should exist in the share memory(shm),the shm will
 *#  Author:lishiming2007@gmail.com
*####################################################################################*/
#ifndef	__H_SHM_CIRCULAR_ARRAY_HEADER
#define	__H_SHM_CIRCULAR_ARRAY_HEADER


#ifndef MAX_PROC_NUM
#define	MAX_PROC_NUM 32
#endif

#ifndef SHM_TRUE
#define SHM_TRUE 1
#endif

#ifndef SHM_FAIL
#define SHM_FALSE -1
#endif

/*
	***************************************************************
		i will change block_info_t*  to  void*

		i use this block_info_t * because this interface is only inner interface.
  	***************************************************************
  */

typedef struct _block_info
{
	unsigned int	block_size;

	unsigned int	block_start;

	unsigned int	block_end;

	unsigned int	is_splits;

	char	flags[MAX_PROC_NUM];
  
}block_info_t;


typedef struct _circular_array
{
	int	head;

	int	tail;

	block_info_t	*pbase;

}circular_array_t;


int init_queue(circular_array_t *pqueue, block_info_t *pbase);


int en_queue(circular_array_t *pqueue, block_info_t *value,  unsigned int queue_size);


int out_queue(circular_array_t *pqueue, unsigned int queue_size);


int is_full(circular_array_t *pqueue, unsigned int queue_size);


int is_empty(circular_array_t *pqueue);

#endif
