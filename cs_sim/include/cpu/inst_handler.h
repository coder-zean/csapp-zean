#ifndef INST_HANDLER_GUARD
#define INST_HANDLER_GUARD

#include "cpu/instruction.h"

void (*handler_table[INST_TYPE_NUM])(uint64_t, uint64_t* dst);

void mov_handler(uint64_t src, uint64_t* dst);
void add_handler(uint64_t src, uint64_t* dst);
void sub_handler(uint64_t src, uint64_t* dst);
void push_handler(uint64_t src, uint64_t* dst);
void pop_handler(uint64_t src, uint64_t* dst);

// 初始化指令执行函数表
void init_handler_table();

#endif