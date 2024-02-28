#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"

void kern_init(); //kernel initialization function

multiboot_t *glb_mboot_ptr;
char kern_stack [STACK_SIZE];

// kernel use tmp page and PDE
// page align
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t*) 0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t*) 0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high= (pgd_t*) 0x3000;

__attribute__((section(".init.text"))) void kern_entry(){
    pgd_tmp[0] = (uint32_t) pte_low | PAGE_PRESENT | PAGE_WRITE;
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t) pte_high | PAGE_PRESENT | PAGE_WRITE;
    //map 4MB
    int i;
    for(i=0;i<1024;i++){
        pte_low = (i<<12) | PAGE_PRESENT | PAGE_WRITE;
    }
    //0-4MB->c000-c4MB
     for(i=0;i<1024;i++){
        pte_high = (i<<12) | PAGE_PRESENT | PAGE_WRITE;
    }
    // 设置临时页表
	asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));

	uint32_t cr0;

	// 启用分页，将 cr0 寄存器的分页位置为 1 就好
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0" : : "r" (cr0));
	
	// 切换内核栈
	uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
	asm volatile ("mov %0, %%esp\n\t"
			"xor %%ebp, %%ebp" : : "r" (kern_stack_top));

	// 更新全局 multiboot_t 指针
	glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

	// 调用内核初始化函数
	kern_init();
}
void kern_init (){
    init_debug();
    init_gdt();
    init_idt();
    console_clear();
    printk_color(rc_black,rc_cyan,"HALO, Os Kernel\n");
    init_timer(200);

    // asm volatile("sti");
    printk("kernel in memory start: 0x%08X\n",kern_start);
    printk("kernel in memory end: 0x%08X\n",kern_end);
    printk("kernel in memory used: %d KB \n\n",(kern_end-kern_start+1023)/1024);
    show_memory_map();
    init_pmm();

    uint32_t  alloc_addr = NULL;
    printk_color(rc_black,rc_light_brown,"Test Phy memory alloc:\n");
    alloc_addr = pmm_alloc_page();
    printk_color(rc_black,rc_light_brown,"Alloc Phy memory addr: 0x%08X\n",alloc_addr);
    alloc_addr = pmm_alloc_page();
    printk_color(rc_black,rc_light_brown,"Alloc Phy memory addr: 0x%08X\n",alloc_addr);
    alloc_addr = pmm_alloc_page();
    printk_color(rc_black,rc_light_brown,"Alloc Phy memory addr: 0x%08X\n",alloc_addr);
    alloc_addr = pmm_alloc_page();
    printk_color(rc_black,rc_light_brown,"Alloc Phy memory addr: 0x%08X\n",alloc_addr);
    pmm_free_page(alloc_addr);
    alloc_addr = pmm_alloc_page();
    printk_color(rc_black,rc_light_brown,"Alloc Phy memory addr: 0x%08X\n",alloc_addr);
    // panic("test");
    // console_write_color("HALO,OS kernel!\n",rc_black,rc_green);
    // return 0;
    while(1){
        asm volatile("hlt");
    }
}