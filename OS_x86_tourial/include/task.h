#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "types.h"
#include "pmm.h"
#include "vmm.h"

typedef 
enum task_state {
    TASK_UNINIT = 0, //未初始化
    TASK_SLEEPING =1, //睡眠中
    TASK_RUNNABLE =2, //可运行
    TASK_ZOMBIE =2,  //僵尸状态
}task_state;

struct context
{
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
};

//进程内存地址结构
struct mm_struct
{
    pgd_t *pgd_dir;      //进程页表
};

//进程控制块  PCB
struct task_struct
{
    volatile task_state state ;      // 进程当前状态
    pid_t    pid;                    // 进程标识符
    void     *stack;                 // 进程的内核栈地址
    struct   mm_struct *mm;          // 当前进程的内存地址印象
    struct   context context;        // 进程切换需要的上下文信息
    struct   task_struct *next;      // 链表指针
};

//全局 pid 值
extern pid_t  now_pid;

int32_t   kernel_thread(int (*fn) (void*), void *arg);

//线程退出函数
void kthread_exit();

#endif