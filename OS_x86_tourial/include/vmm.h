#ifndef INCLUDE_VMM_H
#define INCLUDE_VMM_H

#include "types.h"
#include "idt.h"
#include "pmm.h"

#define PAGE_OFFSET      0xC0000000
/**
 * P--位0是存在（Present）标志，用于指明表项对地址转换是否有效。P=1表示有效；P=0表示无效。
 * 在页转换过程中，如果说涉及的页目录或页表的表项无效，则会导致一个异常。
 * 如果P=0，那么除表示表项无效外，其余位可供程序自由使用。
 * 例如，操作系统可以使用这些位来保存已存储在磁盘上的页面的序号。
 */
#define PAGE_PRESENT 	0x1

/** 
 * R/W--位1是读/写（Read/Write）标志。如果等于1，表示页面可以被读、写或执行。
 * 如果为0，表示页面只读或可执行。
 * 当处理器运行在超级用户特权级（级别0、1或2）时，则R/W位不起作用。
 * 页目录项中的R/W位对其所映射的所有页面起作用。
 */
#define PAGE_WRITE 	0x2

/**
 * U/S--位2是用户/超级用户（User/Supervisor）标志。
 * 如果为1，那么运行在任何特权级上的程序都可以访问该页面。
 * 如果为0，那么页面只能被运行在超级用户特权级（0、1或2）上的程序访问。
 * 页目录项中的U/S位对其所映射的所有页面起作用。
 */
#define PAGE_USER 	0x4

//vm page size
#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000

//get pde
#define PGD_INDEX(x) (((x)>>22)&0x3FF)
//get pte
#define PTE_INDEX(x) (((x)>>12)&0x3FF)
//get page offset
#define OFFSET_INDEX(x) ((x) & 0xFFF)

//data type pgd
typedef uint32_t pgd_t;
//page date
typedef uint32_t pte_t;
//page members
#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t))
//pte members
#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t))

//map 512MB pte num
#define PTE_COUNT 128

//kernel pde 
extern pgd_t pgd_kern[PGD_SIZE];

//initilization vmm
void init_vmm();
//switch PGD
void switch_pgd(uint32_t pd);
//flags point to page priviledge, pa->va
void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);
//va->pa =1, pa !=null 
uint32_t get_mapping(pgd_t *pgd_now,uint32_t va, uint32_t *pa);
//page fault
void page_fault(pt_regs *regs);


#endif