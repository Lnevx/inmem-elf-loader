#
# https://wiki.osdev.org/System_V_ABI#i386
#
    .text
    .globl  a_syscall
    .type   a_syscall,@function
a_syscall:
    push %ebx
    push %esi
    push %edi
    push %ebp

    mov 20(%esp), %eax
    mov 24(%esp), %ebx
    mov 28(%esp), %ecx
    mov 32(%esp), %edx
    mov 36(%esp), %esi
    mov 40(%esp), %edi
    mov 44(%esp), %ebp
    int $0x80

    pop %ebp
    pop %edi
    pop %esi
    pop %ebx
    ret
