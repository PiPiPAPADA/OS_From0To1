# OS_From0To1
建立这个仓库的目的是为了更好的学习操作系统，仅仅看书确实有点枯燥，这个例子比较简单，结合体系结构的知识，动手确实比较快
## Use QEMU & WSL
主要参考这个仓库 https://github.com/ruiers/os-tutorial-cn

## WSL + QEMU + gdb debug
https://wiki.osdev.org/How_Do_I_Use_A_Debugger_With_My_OS
qemu -s -S binfile, 设置gdb 的断点
使用gdb load elf, 读取symbol file