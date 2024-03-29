/*
 * =====================================================================================
 *
 *       Filename:  task.h
 *
 *    Description:  进程管理相关的定义
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "type.h"
#include "proto.h"

// 进程状态描述
typedef
enum task_state {
	TASK_RUNNING 	 = 0, 	// 睡眠中
	TASK_READY		 = 1, 	// 可运行(也许正在运行)
	TASK_BLOCKED     = 2, 	// 僵尸状态
} task_state;

// 内核线程的上下文切换保存的信息
struct context {
	uint32_t esp;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t esi;
	uint32_t edi;
	uint32_t eflags;
};

// 进程内存地址结构
struct mm_struct {
	pgd_t *pgd_dir; 	// 进程页表
};

// 进程控制块 PCB 
struct task_struct {
	volatile task_state state; 	// 进程当前状态
	pid_t 	 pid; 			// 进程标识符
	void  	*stack; 		// 进程的内核栈地址
	struct mm_struct *mm; 		// 当前进程的内存地址映像
	struct context context; 	// 进程切换需要的上下文信息
	struct task_struct *next; 	// 链表指针
	int priority;
	int count;
    int period;
	int interval;
};

// 全局 pid 值
extern pid_t now_pid;

// 内核线程创建
int32_t kernel_thread(int (*fn)(void *), int priority, void *arg, int count);


// 线程退出函数
void kthread_exit();

#endif 	// INCLUDE_TASK_H_

