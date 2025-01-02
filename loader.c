#include <stddef.h>
#include <sys/types.h>

#include "asm.h"
#include "elfs.h"
#include "syscalls.h"
#include "utils.h"

#define PAGE_SIZE   4096
#define ALIGN       (PAGE_SIZE - 1)
#define ROUND_PG(x) (((x) + (ALIGN)) & ~(ALIGN))
#define TRUNC_PG(x) ((x) & ~(ALIGN))
#define PFLAGS(x)                                                     \
    ((((x) & PF_R) ? PROT_READ : 0) | (((x) & PF_W) ? PROT_WRITE : 0) \
     | (((x) & PF_X) ? PROT_EXEC : 0))
#define LOAD_ERR   ((void *)-1)
#define STACK_SIZE 0x21000

extern char __stop_text[];

static void *loadelf_anon(void *elf_addr, Elf_Ehdr *ehdr, Elf_Phdr *phdr) {
    size_t minva, maxva;

    minva = (size_t)-1;
    maxva = 0;

    for (Elf_Phdr *iter = phdr; iter < &phdr[ehdr->e_phnum]; iter++) {
        if (iter->p_type != PT_LOAD) continue;
        if (iter->p_vaddr < minva) minva = iter->p_vaddr;
        if (iter->p_vaddr + iter->p_memsz > maxva) maxva = iter->p_vaddr + iter->p_memsz;
    }

    minva = TRUNC_PG(minva);
    maxva = ROUND_PG(maxva);

    void *base = s_mmap(0, maxva - minva, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == (void *)-1) return LOAD_ERR;
    s_munmap(base, maxva - minva);

    int flags = MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE;
    /* Now map each segment separately in precalculated address. */
    for (Elf_Phdr *iter = phdr; iter < &phdr[ehdr->e_phnum]; iter++) {
        size_t off;

        if (iter->p_type != PT_LOAD) continue;

        off = iter->p_vaddr & ALIGN;
        void *start = base + TRUNC_PG(iter->p_vaddr);
        ssize_t sz = ROUND_PG(iter->p_memsz + off);

        void *p = s_mmap(start, sz, PROT_WRITE, flags, -1, 0);
        if (p == (void *)-1) goto err;

        memcpy(p + off, elf_addr + iter->p_offset, iter->p_filesz);
        s_mprotect(p, sz, PFLAGS(iter->p_flags));
    }

    return base;
err:
    s_munmap(base, maxva - minva);
    return LOAD_ERR;
}

void fini() {
    s_exit(0);
}

__attribute__((section(".text._start"))) void _start() {
    void *elf_addr;
    Elf_Ehdr *ehdr;
    Elf_Phdr *phdr, *iter;

    ehdr = elf_addr = &__stop_text;
    phdr = elf_addr + ehdr->e_phoff;

    void *base = loadelf_anon(elf_addr, ehdr, phdr);
    void (*entry)(void) = ehdr->e_entry + base;

    Elf_auxv_t av[2];
    av[0] = (Elf_auxv_t){AT_RANDOM, (unsigned long)base};
    av[1] = (Elf_auxv_t){AT_NULL, 0};

    a_trampo(entry, &av, sizeof(av), &fini);
}
