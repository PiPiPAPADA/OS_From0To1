# os_x86_simple
实现简单的内核
## makefile 
    patsubst 替换
    NASM -F stabs elf stabs debug format for linux https://nasm.us/xdoc/2.14.01rc3/html/nasmdoc2.html
    link use script:
        set entry address: start 起始段地址， 1MB, .text段（代码段）已初始化数据段.data, 未初始化数据段.bss，采用4096的页对齐
        gcc的rodata段放置的是常量
