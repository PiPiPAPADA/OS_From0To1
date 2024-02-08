#include "console.h"
#include "debug.h"

int kernel_entry(){
    init_debug();
    console_clear();
    printk_color(rc_black,rc_cyan,"HALO, Os Kernel\n");
    panic("test");
    // console_write_color("HALO,OS kernel!\n",rc_black,rc_green);
    return 0;
}