#
# https://wiki.osdev.org/System_V_ABI#x86-64
#
    .text
    .align  4
    .globl  a_syscall
    .type   a_syscall,@function
a_syscall:
    mov %rdi,    %rax
    mov %rsi,    %rdi
    mov %rdx,    %rsi
    mov %rcx,    %rdx
    mov %r8,     %r10
    mov %r9,     %r8
    mov 8(%rsp), %r9
    syscall
    ret
