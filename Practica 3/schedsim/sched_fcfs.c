#include "sched.h"

static task_t* pick_next_task_fcfs(runqueue_t* rq,int cpu)
{
	task_t* t=head_slist(&rq->tasks); //Cogemos el primer elemento de la lista (sin borrarlo)

	/* Current is not on the rq -> let's remove it */
	if (t) { //Si hay elementos en la cola 
		remove_slist(&rq->tasks,t);
		t->on_rq = FALSE; //Marcamos que la tarea ya no esta en la cola de tarea
		rq->cur_task = t; //Le indicamos a la cola cual es la tarea actual
		rq->nr_runnable--;	
	}

	return t;
}

/*static int compare_tasks_cpu_burst(void *t1,void *t2)
{
	task_t* tsk1=(task_t*)t1;
	task_t* tsk2=(task_t*)t2;
	return tsk1->runnable_ticks_left-tsk2->runnable_ticks_left;
}*/

static void enqueue_task_fcfs(task_t* t,int cpu, int runnable)
{
	runqueue_t* rq=get_runqueue_cpu(cpu); //Coge la runqueue de la CPU

	if (t->on_rq || is_idle_task(t))
		return;
	
	insert_slist(&rq->tasks, t); //Inserta en la lista la tarea
	
	t->on_rq = TRUE; //Indicamos que la tarea esta en la runqueue


	/* If the task was not runnable before, update the number of runnable tasks in the rq*/
	if (!runnable) {
		rq->nr_runnable++;
		t->last_cpu = cpu;
	}
}

static task_t* steal_task_fcfs(runqueue_t* rq,int cpu)
{
	task_t* t=tail_slist(&rq->tasks); //Saco la tarea de la cola lista

	if (t) {
		remove_slist(&rq->tasks,t); //Eliminamos la tarea de la lista
		t->on_rq=FALSE; //Marcamos que la tarea ya no esta en la runqueue
		rq->nr_runnable--;
	}
	return t;
}


sched_class_t fcfs_sched= {
	.pick_next_task=pick_next_task_fcfs,
	.enqueue_task=enqueue_task_fcfs,
	.steal_task=steal_task_fcfs
};

