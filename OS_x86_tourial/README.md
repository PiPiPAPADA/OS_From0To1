# os_x86_simple
实现简单的内核
## makefile 
    patsubst 替换
    NASM -F stabs elf stabs debug format for linux https://nasm.us/xdoc/2.14.01rc3/html/nasmdoc2.html
    link use script:
        set entry address: start 起始段地址， 1MB, .text段（代码段）已初始化数据段.data, 未初始化数据段.bss，采用4096的页对齐
        gcc的rodata段放置的是常量
        gcc flags
            -m32 32bit 代码
            -nostdinc 不包含c语言的标准库里的文件
            -fno-builtin gcc 不主动使用自己的内建函数，除非显式声明
            -fno-stack-protector 不使用栈保护保护检测
            -fno-pic 生成的代码是位置无关码
## floppy disk
    Linux需要制作一个GRUB的启动盘，目前使用的是hx制作的，所以makefile中的target是hx_kernel https://github.com/hurley25/hurlex-doc/tree/master
## 库函数
    va_list -> char * gcc 内建
    va_start(p, first) (p = (va_list)&first + sizeof(first))
    va_arg(p, next) (*(next*)((p += sizeof(next) ) 􀀀 sizeof(next)))
    va_end(p) (p = (va_list)NULL)
## debug info
    boot    将ebx存储的指针放在mult boot struct 指针
    ELF 从multboot中获取信息
## GDT
    保护模式，CPU 4种模式，实模式，保护模式，虚拟8086模式，SMM模式
    保护模式，增强多工和系统稳定度，比如内存保护，分页系统以及硬件支持的虚拟内存
    引入段描述，访问内存通过segment寄存器，对内存段描述需要8个字节，data/code/sys 
    寄存器因为放不下所有的信息，需要用到GDT来表示
    386 5个控制寄存器，CR0-3， CR8，用以进入保护模式
## MM
    物理内存管理，Linux 采用伙伴算法，伙伴算法在申请和释放物理页框的时候会对物理页框进行合并操作，尽可能的保证可用物理内存的连续性
	内存碎片分为内部碎片和外部碎片两种。内部碎片就是已经被分配出去却不能被利用的内存空间，比如我们为了管理方便，按照4KB内存块进行管理。此时任何申请内存的操作都会以4KB的倍数返回内存块。即使申请1个字节也返回指向4KB大的内存块的指针，这样的话造成了分配出去的内存没有被有效利用，而且剩余空间无法分配给其它进程（因为最小的管理单位是4KB）。
	外部碎片是指内存频繁请求和释放大小不同的连续页框后，导致在已分配页框块周围分散了许多小块空闲的页框，尽管这些空闲页框的总数可以满足接下来的请求，但却无法满足一个大块的连续页框。
    页管理机制引入，PAGE->memory 
    虚拟页面 PAGE size 4KB, 物理内存也按照页管理，VA->PA的页目录和页表组成二级页表，页目录的地址位置在CR3寄存器中
    linux中采取的映射，把物理地址0从虚拟地址0xC000000（3GB）处开始往上映射，只用512MB, 所以3-4G完全映射全部的物理地址，物理地址+0xC000000 = 内核虚拟地址
    VMA (virtual memory address) LMA(load memory address) VMA是链接器生成可执行文件时的偏移计算地址，LMA是区段所载入内存的实际地址，通常情况下，VMA = LMA
    按页管理，分配小内存的时候比较容易造成内存碎片
    堆管理，小内存分配，简单的分配内存外，考虑内存释放的时候，对连续的内存进行合并，简单的侵入式链表管理
