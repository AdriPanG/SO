#include "sched.h"

static task_t* pick_next_task_prioTR(runqueue_t* rq,int cpu)
{
	task_t* t=head_slist(&rq->tasks);

	/* Current is not on the rq -> let's remove it */
	if (t) { //Si hay elementos en la cola
		remove_slist(&rq->tasks,t); //Borramos la tarea de la lista
		t->on_rq = FALSE; //Marcamos que la tarea ya no esta en la cola de tareas
		rq->cur_task=t; //Le indicamos a la cola cual es la tarea actual
		
		//rq->nr_runnable--;
	}
	
	return t;
}

static int compare_tasks_cpu_burst(void *t1,void *t2)
{
	task_t* tsk1=(task_t*)t1;
	task_t* tsk2=(task_t*)t2;
	return tsk1->prio-tsk2->prio;
}

static void enqueue_task_prioTR(task_t* t,int cpu, int runnable)
{
	runqueue_t* rq=get_runqueue_cpu(cpu); //Coge la runqueue de la CPU

	if (t->on_rq || is_idle_task(t))
		return;
	
	sorted_insert_slist_front(&rq->tasks, t, 1, compare_tasks_cpu_burst);  //Push task
	//insert_slist(&rq->tasks, t); //Inserta en la lista la tarea
	if(t->prio==0)
		rq->need_resched=TRUE;
	t->on_rq = TRUE; //Indicamos que la tarea esta en la runqueue


	/* If the task was not runnable before, update the number of runnable tasks in the rq*/
	if (!runnable) {
		rq->nr_runnable++;
		t->last_cpu = cpu;
	}
}

static void task_tick_prioTR(runqueue_t *rq , int cpu){
	
	task_t* current=rq->cur_task; //Obtenemos la tarea actual
    
	if (is_idle_task(current)) //Si la tarea actual esta libre salimos
        return;
        
	task_t* cabecera = head_slist(&rq->tasks);
	
   // if(cabecera != NULL && cabecera->prio == 0 && current->prio != 0){
   if(current->runnable_ticks_left==1){
		rq->need_resched=TRUE; //Avisamos que vamos a replanificar
		if (current->runnable_ticks_left==1 && current->prio==0 ) //Comprobamos que es el ultimo tick de la rafaga de CPU
			rq->nr_runnable--; //Decrementamos el numero de tareas
	}
	
	
}

static task_t* steal_task_prioTR(runqueue_t* rq,int cpu)
{
	task_t* t=tail_slist(&rq->tasks);

	if (t) {
		remove_slist(&rq->tasks,t);
		t->on_rq=FALSE;
		rq->nr_runnable--;
	}
	return t;
}


sched_class_t prioTR_sched= {
	.pick_next_task=pick_next_task_prioTR,
	.enqueue_task=enqueue_task_prioTR,
	.steal_task=steal_task_prioTR,
	.task_tick=task_tick_prioTR
};
