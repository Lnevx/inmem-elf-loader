#include "syscalls.h"

#include <sys/syscall.h>

#include "asm.h"

#define SYSCALL(name, ...) a_syscall(SYS_##name, __VA_ARGS__)

#define DEF_SYSCALL2(ret, name, t1, a1, t2, a2) \
    ret s_##name(t1 a1, t2 a2) {                \
        return (ret)SYSCALL(name, a1, a2);      \
    }
#define DEF_SYSCALL3(ret, name, t1, a1, t2, a2, t3, a3) \
    ret s_##name(t1 a1, t2 a2, t3 a3) {                 \
        return (ret)SYSCALL(name, a1, a2, a3);          \
    }

DEF_SYSCALL2(int, munmap, void *, addr, size_t, length);
DEF_SYSCALL3(int, mprotect, void *, addr, size_t, len, int, prot);

void *s_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
#ifdef SYS_mmap2
    return (void *)SYSCALL(mmap2, addr, length, prot, flags, fd, offset >> 12);
#else
    return (void *)SYSCALL(mmap, addr, length, prot, flags, fd, offset);
#endif
}
