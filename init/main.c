#include "../include/main.h"
#include "../include/task.h"

int task1(void *arg)
{
    while (1)
    {
        printk("A");
        wait(100);
    }
    return 0;
}

int task2(void *arg)
{
    while (1)
    {
        printk("B");
        wait(100);
    }
    return 0;
}

int task3(void *arg)
{

    while (1)
    {
        printk("C");
        wait(100);
        
    }
    return 0;
}

int rttask()
{
    printk("\nAre you ok?\n");
    return 0;
}

void start_kernel()
{	
    init_all();
    int pidA=-1, pidB=-1, pidC=-1;
    pidA = kernel_thread(task1, 20, NULL, 40);
    pidB = kernel_thread(task2, 40, NULL, 40);
    pidC = kernel_thread(task3, 60, NULL, 40);
    //kernel_thread(rttask,10,NULL);
    scopeSet(pidA, pidB, pidC);
    //kernel_thread(task3,20,NULL);
    while(1){;}
}


