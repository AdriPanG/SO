#include "barrier.h"
#include <errno.h>


#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads)
{
	return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	return pthread_barrier_wait(barrier);
}

#else


/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	/* Initialize fields in sys_barrier_t
	     ... To be completed ....
	*/
	if(!barrier)pthread_mutex_init(&barrier->mutex, NULL); //Si no hay barrera inicializamos el mutex
	if(!barrier)pthread_cond_init(&barrier->cond,NULL); //Si no hay barrera inicializamos la variable condicional
	barrier->max_threads = nr_threads; //Decimos cual es el maximo de hilos de la barrera
	barrier->nr_threads_arrived[0] = 0;
	barrier->nr_threads_arrived[1] = 0; 
	return 0;
}

/* Destroy barrier resources */
int sys_barrier_destroy(sys_barrier_t *barrier)
{ 
	pthread_mutex_destroy(&barrier->mutex); //Destruimos el mutex
    pthread_cond_destroy(&barrier->cond); //Destruimos el cond
	return 0;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{
	/* Implementation outline:
	   - Every thread arriving at the barrier adquires the lock and increments the nr_threads_arrived
	    counter atomically
	     * In the event this is not the last thread to arrive at the barrier, the thread
	       must block in the condition variable
	     * Otherwise...
	        1. Reset the barrier state in preparation for the next invocation of sys_barrier_wait() and
	        2. Wake up all threads blocked in the barrier
	   - Don't forget to release the lock before returning from the function

	    ... To be completed ....
	*/
	pthread_mutex_lock(&barrier->mutex);
    if((barrier->nr_threads_arrived[0] + barrier->nr_threads_arrived[1]) % 2 == 0) { 
		++(barrier->nr_threads_arrived[0]);
	} else
		++(barrier->nr_threads_arrived[1]);
    if((barrier->nr_threads_arrived[0] + barrier->nr_threads_arrived[1])==barrier->max_threads) {
		pthread_cond_broadcast(&barrier->cond);
		sys_barrier_init(barrier, barrier->max_threads);
	} else 
		pthread_cond_wait(&barrier->cond, &barrier->mutex);
		
	pthread_mutex_unlock(&barrier->mutex);
    
	return 0;
}

#endif /* POSIX_BARRIER */
