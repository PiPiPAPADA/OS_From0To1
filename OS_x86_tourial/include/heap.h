#ifndef INCLUDE_HEAP_H_
#define INCLUDE_HEAP_H_

#include "types.h"

#define HEAP_START   0xE0000000  //内核页表没有使用的空闲区域

typedef
struct header
{
    struct header *prev;
    struct header *next;
    uint32_t allocated :1 ; //内存块是否被申请
    uint32_t length  : 31 ; // 当前内存的长度
}header_t;

void init_heap();

//内存申请
void *kmalloc(uint32_t len);

//内存释放
void kfree(void *p);

//测试内核堆申请释放
void test_heap();


#endif