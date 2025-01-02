#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stddef.h>
#include <sys/mman.h>

void s_exit(int status);
void *s_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int s_munmap(void *addr, size_t length);
int s_mprotect(void *addr, size_t len, int prot);

#endif /* SYSCALLS_H */
