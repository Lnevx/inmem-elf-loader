    .text
    .globl  a_trampo
    .type   a_trampo,@function
a_trampo:
    mov 4(%esp), %ebp
    mov 8(%esp), %edi
    mov 12(%esp), %edx

    sub %edx, %esp
    mov %esp, %esi
    push %edx
    push %edi
    push %esi
    call memcpy
    add $12, %esp

    xor %eax, %eax
    push %eax         # envp[0] = NULL
    push %eax         # argv[0] = NULL
    push %eax         # argc = 0
    xor %edx, %edx    # (*fini)(void) = NULL

    jmp *%ebp
