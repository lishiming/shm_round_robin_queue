/*##################################################################################*//*

 *#  @FILE: shm_circular_array.h
 *#  DESCRIPTION : 
      in this file,I use an array to realize a round-robin queue, I don't use the link list
      because the round-robin queue should exist in the share memory(shm),the shm will be 
      getted in init,if each time get a new shm for a new block,a lot of system calls will
      happen,so I use a given shm to realize a queue.
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


/**
 * 
 * STRUCT NAME: block_info_t
 * DESCRIPTION: it contains the info of each data block 
 */
 
typedef struct _block_info
{
	unsigned int	block_size; /*the size of this data block*/

	unsigned int	block_start; /*offset of the data block'start to the start of shm*/

	unsigned int	block_end; /* offset of the data block'end to the start of shm*/

	unsigned int	is_splits; /*a data block may locate in both the end and the start of the shm,
					if this happens,this param will be SHM_SPLITS,contrary SHM_NO_SPLITS*/

	char	flags[MAX_PROC_NUM]; /* this array marks if this data block is readed by the process,process x read,
					flags[x]=FLAG_TRUE, default:FLAG_FALSE*/
  
}block_info_t;



/**
 * 
 * STRUCT NAME: circular_array_t
 * DESCRIPTION: info of the  circular_array
 * 	   
 * 	i will change block_info_t*  to  void*,i use this block_info_t * because this interface is 
 * 	only inner interface. but I known this isn't beautiful.
 */
 
typedef struct _circular_array
{
	int	head; /*offset of the oldest data in the queue to the pbase*/

	int	tail; /*offset of the newest data in the queue to the pbase*/

	block_info_t	*pbase; /*the start of the circular_array*/

}circular_array_t;


/**
 * 
 * METHOD: init_queue
 * DESCRIPTION: init the info of queue  
 * @param :pqueue, the addrss of array
 * @param :pbase, the first block should be located here
 * @return :SHM_TRUE,success，if fail，SHM_FALSE
 */
 
int init_queue(circular_array_t *pqueue, block_info_t *pbase);


/**
 * 
 * METHOD: en_queue
 * DESCRIPTION: insert a block to the queue  to the tail
 * @param :pqueue, the addrss of array
 * @param :value, the block's info
 * @param :queue_size,the size of the queue
 * @return :SHM_TRUE,success，if fail，SHM_FALSE
 */
 
int en_queue(circular_array_t *pqueue, block_info_t *value,  unsigned int queue_size);


/**
 * 
 * METHOD: out_queue
 * DESCRIPTION:  the head dequeue
 * @param :pqueue, the addrss of array
 * @param :queue_size,the size of the queue
 * @return :SHM_TRUE,success，if fail，SHM_FALSE
 */
 
int out_queue(circular_array_t *pqueue, unsigned int queue_size);


/**
 * 
 * METHOD: is_full
 * DESCRIPTION:  jude if the queue is full
 * @param :pqueue, the addrss of array
 * @param :queue_size,the size of the queue
 * @return :SHM_TRUE,if queue is full，if not，SHM_FALSE
 */
 
int is_full(circular_array_t *pqueue, unsigned int queue_size);


/**
 * 
 * METHOD: is_empty
 * DESCRIPTION:  jude if the queue is empty
 * @param :pqueue, the addrss of array
 * @return :SHM_TRUE,if queue is empty，if not，SHM_FALSE
 */
int is_empty(circular_array_t *pqueue);

#endif
