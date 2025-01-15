#ifndef IEL_ASM_H
#define IEL_ASM_H

long a_syscall(int nr, ...);
void a_trampo(void (*entry)(void), void *auxv, size_t auvx_size);

#endif /* IEL_ASM_H */
