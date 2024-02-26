#ifndef INCLUDE_PMM_H
#define INCLUDE_PMM_H

#include "multiboot.h"

//内核文件的起始结束地址，在链接器中要求链接器重新定义
extern uint8_t kern_start[];
extern uint8_t kern_end[];
extern uint32_t phy_page_count;

//输出BIOS提供的物理内存布局
void show_memory_map();
void init_pmm();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t p);

//thread stack size
#define STACK_SIZE 8192

//max phy memory (512MB)
#define PMM_MAX_SIZE 0x20000000
//phy memory size 
#define PMM_PAGE_SIZE 0x1000
//phy page num
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)
//page mask 4096 align
#define PHY_PAGE_MASK 0xFFFFF000


#endif