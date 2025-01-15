    .text
    .align  4
    .globl  a_trampo
    .type   a_trampo,@function
a_trampo:
    sub %rdx, %rsp
    mov %rdi, %rbp
    mov %rsp, %rdi
    call memcpy       # copy av

    xor %eax, %eax
    push %rax         # envp[0] = NULL
    push %rax         # argv[0] = NULL
    push %rax         # argc = 0
    xor %edx, %edx    # (*fini)(void) = NULL

    jmp *%rbp
