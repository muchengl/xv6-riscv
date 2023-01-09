        # qemu -kernel loads the kernel at 0x80000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
.global _entry
_entry:
        # set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)

        la sp, stack0    # stack0是一个label，sp->stack0
        li a0, 1024*4    # a0=1024*4
        csrr a1, mhartid # 读寄存器mhartid到a1寄存器，mhartid是Hart ID Register：运行当前代码硬件线程(hart)的ID
        addi a1, a1, 1   # a1+=1，
        mul a0, a0, a1   # a0=a0*a1
        add sp, sp, a0   # sp=sp+a0 ，sp指向当前准备执行的指令的内存位置，此处sp指向栈顶

        # jump to start() in start.c
        call start
spin:
        j spin
