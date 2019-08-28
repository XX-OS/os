/*
 * =====================================================================================
 *
 *       Filename:  sched.c
 *
 *    Description:  线程调度函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include "../include/sched.h"
#include "../include/heap.h"
#include "../include/proto.h"
#include "../include/printk.h"
#include "../include/scope.h"
extern unsigned int stack_end;
extern unsigned int stack_init;
// 可调度进程链表
struct task_struct *running_proc_head = NULL;

// 等待进程链表
struct task_struct *wait_proc_head = NULL;

// 当前运行的任务
struct task_struct *current = NULL;

struct 
{
	struct task_struct * __Rdy_Queue[256];
	uint16_t 			 __Rdy_Queue_Len; 

	struct task_struct * __Blk_Queue[256];
	uint16_t 			 __Blk_Queue_Len;
} Scheduler;

void Init_Scheduler()
{
	Scheduler.__Rdy_Queue_Len = 0;
	Scheduler.__Blk_Queue_Len = 0;


	current = (struct task_struct *)kmalloc(STACK_SIZE);

	current->state = TASK_RUNNING;
	current->pid = now_pid++;
	current->stack = current; 	// 该成员指向栈低地址
	current->mm = NULL; 		// 内核线程不需要该成员

	current->priority = 1;
	current->count = current->priority;

	// 单向循环链表
	current->next = current;

	running_proc_head = current;
}


struct task_struct * running_task = NULL;

void Insert_Rdy_Queue(struct task_struct * process)
{
	int i = 0, j = 0;
	++Scheduler.__Rdy_Queue_Len;
	i = Scheduler.__Rdy_Queue_Len - 1;
	while (i != 0)
	{
		j = (i - 1)/2;
		if (process->priority < Scheduler.__Rdy_Queue[j]->priority)
			break;
		
		Scheduler.__Rdy_Queue[i] = Scheduler.__Rdy_Queue[j];
		i = j;  
	}

	Scheduler.__Rdy_Queue[i] = process;
}

void Pop_Rdy_Queue()
{
	int i = 1, j = 0;
	--Scheduler.__Rdy_Queue_Len;
	while (i < Scheduler.__Rdy_Queue_Len)
	{
		if (i + 1 < Scheduler.__Rdy_Queue_Len && 
			Scheduler.__Rdy_Queue[i] < Scheduler.__Rdy_Queue[i + 1])
			++i;
		if (Scheduler.__Rdy_Queue[Scheduler.__Rdy_Queue_Len]->priority > 
			Scheduler.__Rdy_Queue[i]->priority)
			break;
		Scheduler.__Rdy_Queue[j] = Scheduler.__Rdy_Queue[i];
		j = i; 	
		i = 2*j + 1;		
	}
	Scheduler.__Rdy_Queue[j] = Scheduler.__Rdy_Queue[Scheduler.__Rdy_Queue_Len];
}

void Running_to_Blocked()
{
	current->count = current->period;
	Scheduler.__Blk_Queue[Scheduler.__Blk_Queue_Len++] = current;
	change_task_to(Scheduler.__Rdy_Queue[0]);
	Pop_Rdy_Queue();
}

void Blocked_to_Ready()
{
	int i;
	for (i = 0; i < Scheduler.__Blk_Queue_Len; ++i)
	{
		--Scheduler.__Blk_Queue[i]->interval;
		if (Scheduler.__Blk_Queue[i]->interval >= 0)
			continue;
		
		Scheduler.__Blk_Queue[i]->interval = 0;
		Insert_Rdy_Queue(Scheduler.__Blk_Queue[i]);
		Scheduler.__Blk_Queue[i] = Scheduler.__Blk_Queue[--Scheduler.__Blk_Queue_Len];
	}
}

void Running_to_Ready()
{
	Insert_Rdy_Queue(current);
	change_task_to(Scheduler.__Rdy_Queue[0]);
	Pop_Rdy_Queue();
}

void schedule()
{
	/* Modifications should be made here
	++++ */
	Blocked_to_Ready();

	if(0 == Scheduler.__Rdy_Queue_Len)
		return;

	if (current->count > 0)
		--current->count;
	else {
		Running_to_Blocked();
		scope_draw(current->pid);
	}

	if (Scheduler.__Rdy_Queue[0]->priority > current->priority)
	{
		Running_to_Ready();
		scope_draw(current->pid);
	}
}

void change_task_to(struct task_struct *next)
{
	if (current != next) {
		struct task_struct *prev = current;
		current = next;

		switch_to(&(prev->context), &(current->context));
	}

}

