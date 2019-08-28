/*
 * =====================================================================================
 *
 *       Filename:  sched.h
 *
 *    Description:  内核的线程调度
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

#include "task.h"

// 可调度进程链表
extern struct task_struct *running_proc_head;

// 等待进程链表
extern struct task_struct *wait_proc_head;

// 当前运行的任务
extern struct task_struct *current;

// 初始化任务调度
void init_sched();

// 任务调度
void schedule();

// 任务切换准备
void change_task_to(struct task_struct *next);

void Init_Scheduler();

// 任务切换
void switch_to(struct context *prev, struct context *next);

void Insert_Rdy_Queue(struct task_struct * process);

void Pop_Rdy_Queue();

void Running_to_Blocked();

void Blocked_to_Ready();

void Running_to_Ready();

#endif 	// INCLUDE_SCHEDULER_H_
