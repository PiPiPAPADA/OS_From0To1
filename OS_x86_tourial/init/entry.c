#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"

int kernel_entry(){
    init_debug();
    init_gdt();
    init_idt();
    console_clear();
    printk_color(rc_black,rc_cyan,"HALO, Os Kernel\n");
    init_timer(200);
    asm volatile("sti");
    // panic("test");
    // console_write_color("HALO,OS kernel!\n",rc_black,rc_green);
    return 0;
}