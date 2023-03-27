#ifndef CPU_GUARD
#define CPU_GUARD

#include "cpu/register.h"
#include "cpu/instruction.h"
#include <stdlib.h>

// 将除IMM格式外的操作数格式解析成虚拟地址vaddr
uint64_t operand2vaddr(operand_t operand);

// 指令周期
void instruction_cycle(inst_t* program, uint64_t inst_num);

#endif