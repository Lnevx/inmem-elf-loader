#                     nr   1    2    3    4    5   6
# User space ABI:          rdi, rsi, rdx, rcx, r8, r9
# Kernel space ABI:   rax, rdi, rsi, rdx, r10, r8, r9

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
