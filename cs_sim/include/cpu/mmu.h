#ifndef MMU_GUARD
#define MMU_GUARD

#include "memory/dram.h"
#include <stdio.h>
#include <stdint.h>

// 虚拟地址转换为物理地址
uint64_t vaddr2paddr(uint64_t vaddr);

#endif