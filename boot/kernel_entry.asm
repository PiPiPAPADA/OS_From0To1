[bits 32]
;protected mode 
[extern main] ;declate that we will be referencing the external symbol main, linker can substitue final address
call main ; Calls the C function. The linker will know where it is placed in memory
jmp $