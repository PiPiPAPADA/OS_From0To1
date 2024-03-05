#include "debug.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"

static void alloc_chunk(uint32_t start, uint32_t len);
static void free_chunk(header_t *chunk);
static void split_chunk(header_t *chunk, uint32_t len);
static void glue_chunk(header_t *chunk);

static uint32_t heap_max = HEAP_START;

static header_t * heap_first;

void init_heap(){
    heap_first = 0;
}


void *kmalloc(uint32_t len){
    //申请内存长度加上管理头的长度
    //因为在内存申请和释放的时候要通过该结构去管理
    len += sizeof(header_t);

    header_t *cur_header = heap_first;
    header_t *prev_header = 0;
    while(cur_header){
        if(cur_header->allocated == 0 && cur_header->length >= len){
            split_chunk(cur_header,len);
            cur_header->allocated = 1;
            return (void*) ((uint32_t)cur_header + sizeof(header_t));
        }
        //推移指针
        prev_header = cur_header;
        cur_header = cur_header->next;
    }

    uint32_t chunk_start;
    //第一次执行该函数初始化内存块起始位置
    if(prev_header){
        chunk_start = (uint32_t) prev_header + prev_header->length;
    } else {
        chunk_start = HEAP_START;
        heap_first = (header_t *) chunk_start;
    }

    alloc_chunk(chunk_start,len);
    cur_header = (header_t *) chunk_start;
    cur_header->prev = prev_header;
    cur_header->next = 0;
    cur_header->allocated = 1;
    cur_header->length = len;

    if(prev_header){
        prev_header->next = cur_header;
    }
    return (void *) (chunk_start + sizeof(header_t));
}

void kfree(void *p){
    header_t *header = (header_t *) ((uint32_t)p - sizeof(header_t));
    header->allocated = 0;
    glue_chunk(header);
}

void alloc_chunk(uint32_t start, uint32_t len){
    while(start + len > heap_max){
        uint32_t page = pmm_alloc_page();
        map(pgd_kern,heap_max,page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += PAGE_SIZE;
    }
}

void free_chunk(header_t *chunk){
    if(chunk->prev == 0){
        heap_first = 0;
    } else {
        chunk->prev->next = 0;
    }

    while((heap_max - PAGE_SIZE) >= (uint32_t)chunk){
        heap_max -= PAGE_SIZE;
        uint32_t page;
        get_mapping(pgd_kern,heap_max,&page);
        unmap(pgd_kern,heap_max);
        pmm_free_page(page);
    }
}

void split_chunk(header_t *chunk, uint32_t len){
    if(chunk->length - len > sizeof (header_t)){
        header_t *new_chunk = (header_t *)((uint32_t)chunk + len);
        new_chunk->prev = chunk;
        new_chunk->next = chunk->next;
        new_chunk->allocated = 0;
        new_chunk->length = chunk->length -len;

        chunk->next = new_chunk;
        chunk->length = len;
    }
}

void glue_chunk(header_t * chunk){
    if(chunk->next && chunk->next->allocated == 0){
        chunk->length =  chunk->length + chunk->next->length;
        if(chunk->next->next){
            chunk->next->next->prev = chunk;
        }
        chunk->next = chunk->next->next;
    }
    if(chunk->prev && chunk->prev->allocated == 0){
        chunk->prev->length =  chunk->prev->length + chunk->length;
        chunk->prev->next = chunk->next;
        if(chunk->next){
            chunk->next->prev = chunk->prev;
        }
        chunk = chunk->prev;
    }
    if(chunk->next == 0){
        free_chunk(chunk);
    }
}

void test_heap()
{
	printk_color(rc_black, rc_magenta, "Test kmalloc() && kfree() now ...\n\n");

	void *addr1 = kmalloc(50);
	printk("kmalloc    50 byte in 0x%X\n", addr1);
	void *addr2 = kmalloc(500);
	printk("kmalloc   500 byte in 0x%X\n", addr2);
	void *addr3 = kmalloc(5000);
	printk("kmalloc  5000 byte in 0x%X\n", addr3);
	void *addr4 = kmalloc(50000);
	printk("kmalloc 50000 byte in 0x%X\n\n", addr4);

	printk("free mem in 0x%X\n", addr1);
	kfree(addr1);
	printk("free mem in 0x%X\n", addr2);
	kfree(addr2);
	printk("free mem in 0x%X\n", addr3);
	kfree(addr3);
	printk("free mem in 0x%X\n\n", addr4);
	kfree(addr4);
}