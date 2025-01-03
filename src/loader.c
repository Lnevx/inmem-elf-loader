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

extern char __stop_text[];

static void *loadelf(void *elf_addr, Elf_Ehdr *ehdr, Elf_Phdr *phdr) {
    int flags, dyn = ehdr->e_type == ET_DYN;
    size_t minva = -1, maxva = 0;

    for (Elf_Phdr *iter = phdr; iter < &phdr[ehdr->e_phnum]; iter++) {
        if (iter->p_type != PT_LOAD) continue;
        if (iter->p_vaddr < minva) minva = iter->p_vaddr;
        if (iter->p_vaddr + iter->p_memsz > maxva) maxva = iter->p_vaddr + iter->p_memsz;
    }

    minva = TRUNC_PG(minva);
    maxva = ROUND_PG(maxva);

    void *hint = dyn ? NULL : (void *)minva;
    flags = dyn ? 0 : MAP_FIXED;
    flags |= MAP_ANONYMOUS | MAP_PRIVATE;

    void *base = s_mmap(hint, maxva - minva, PROT_NONE, flags, -1, 0);
    s_munmap(base, maxva - minva);

    /* Now map each segment separately in precalculated address. */
    for (Elf_Phdr *iter = phdr; iter < &phdr[ehdr->e_phnum]; iter++) {
        size_t off;

        if (iter->p_type != PT_LOAD) continue;

        off = iter->p_vaddr & ALIGN;
        void *start = (dyn ? base : 0) + TRUNC_PG(iter->p_vaddr);
        ssize_t sz = ROUND_PG(iter->p_memsz + off);

        void *p = s_mmap(start, sz, PROT_WRITE, flags, -1, 0);
        memcpy(p + off, elf_addr + iter->p_offset, iter->p_filesz);
        s_mprotect(p, sz, PFLAGS(iter->p_flags));
    }

    return base;
}

void fini() {
    s_exit(0);
}

__attribute__((section(".text._start"))) void _start() {
    void *elf_addr;
    Elf_Ehdr *ehdr = elf_addr = &__stop_text;
    Elf_Phdr *phdr = elf_addr + ehdr->e_phoff;

    void *base = loadelf(elf_addr, ehdr, phdr);
    void (*entry)(void) = ehdr->e_entry + (ehdr->e_type == ET_DYN ? base : 0);

    Elf_auxv_t av[8];
    av[0] = (Elf_auxv_t){AT_PHDR, (size_t)phdr};
    av[1] = (Elf_auxv_t){AT_PHENT, (size_t)ehdr->e_phentsize};
    av[2] = (Elf_auxv_t){AT_PHNUM, (size_t)ehdr->e_phnum};
    av[3] = (Elf_auxv_t){AT_PAGESZ, PAGE_SIZE};
    av[4] = (Elf_auxv_t){AT_ENTRY, (size_t)entry};
    av[5] = (Elf_auxv_t){AT_CLKTCK, 100};
    av[6] = (Elf_auxv_t){AT_RANDOM, (size_t)&_start};
    av[7] = (Elf_auxv_t){AT_NULL, 0};

    a_trampo(entry, &av, sizeof(av), &fini);
}
