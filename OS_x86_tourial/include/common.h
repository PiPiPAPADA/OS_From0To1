#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H
#include "types.h"

// 写port
void outb(uint16_t port, uint8_t value);
//读 port
uint8_t inb(uint16_t port);
// 读一个字
uint16_t inw(uint16_t port);

void enable_intr();
void disable_intr();


#endif