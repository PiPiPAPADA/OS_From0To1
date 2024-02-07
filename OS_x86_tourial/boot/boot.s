; ----------------------------------------------------------------
;
; 	boot.s -- 内核从这里开始
;
;                 这里还有根据 GRUB Multiboot 规范的一些定义
;
; ----------------------------------------------------------------
MBOOT_HEADER_MAGIC      equ       0x1BADB002  ; multiboot magic number
MBOOT_PAGE_ALIGN        equ       1 << 0      ; 0 means 4kB align
MBOOT_MEM_INFO          equ       1 << 1      ; mem_* 包括可用内存的信息，告诉GRUB把内存空间信息包含在multiboot信息结构中

; define multiboot token
MBOOT_HEADER_FLAGS      equ       MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; magic + flags + checksum =0
MBOOT_CHECKSUM          equ       -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; Multiboot 头的分布必须如下表所示：
; ----------------------------------------------------------
; 偏移量  类型  域名        备注
;
;   0     u32   magic       必需
;   4     u32   flags       必需 
;   8     u32   checksum    必需 
;
; 我们只使用到这些就够了，更多的详细说明请参阅 GNU 相关文档
;-----------------------------------------------------------

;-----------------------------------------------------------------------------

[BITS 32] ;32 bit compile

section .text ;

dd  MBOOT_HEADER_MAGIC 
dd  MBOOT_HEADER_FLAGS
dd  MBOOT_CHECKSUM        

[GLOBAL start]
[GLOBAL glb_mboot_ptr] ; 全局的struct multiboot * 变量
[extern kernel_entry]  ; 声明内核c 代码的入口函数

start:
    cli                       ; 关闭所有中断
    mov esp,STACK_TOP      ; 设置栈地址
    mov ebp,0              ; 帧指针修改为0
    and esp,0FFFFFFF0H     ; 16byte 对齐
    mov [glb_mboot_ptr], ebx ; 将ebx中存储的指针放入全局变量
    call kernel_entry         ; 调用内核入口函数
stop:
    hlt                       ; 停机
    jmp  stop                 ; 

section .bss                  ; 未初始化的数据段
stack: 
    resb   32768              ; 内核栈
glb_mboot_ptr:                ; 全局的multiboot 结构体指针
    resb   4

STACK_TOP  equ      $-stack-1 ; 内核栈顶，$是当前地址
