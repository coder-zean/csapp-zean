#include "cpu/mmu.h"

uint64_t vaddr2paddr(uint64_t vaddr) {
    return vaddr % MEM_SIZE;
}