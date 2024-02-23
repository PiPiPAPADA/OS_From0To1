#ifndef INCLUDE_PMM_H
#define INCLUDE_PMM_H

#include "multiboot.h"

//内核文件的起始结束地址，在链接器中要求链接器重新定义
extern uint8_t kern_start[];
extern uint8_t kern_end[];

//输出BIOS提供的物理内存布局
void show_memory_map();

#endif