#include "console.h"
#include "debug.h"
#include "gdt.h"

int kernel_entry(){
    init_debug();
    init_gdt();
    init_idt();
    console_clear();
    printk_color(rc_black,rc_cyan,"HALO, Os Kernel\n");
    asm volatile ("int $0x3");
	asm volatile ("int $0x4");
    // panic("test");
    // console_write_color("HALO,OS kernel!\n",rc_black,rc_green);
    return 0;
}