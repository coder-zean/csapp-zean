#ifndef INSTRUCTION_GUARD
#define INSTRUCTION_GUARD

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "mem/dram.h"

#define INST_TYPE_NUM 100

typedef enum {
    OPERAND_EMPTY,
    OPERAND_MEM,
    OPERAND_IMM,
    OPERAND_REG
} operand_type_t;

typedef struct {
    operand_type_t operand_type;
    int64_t  imm;
    uint64_t reg1;
    uint64_t reg2;
    uint8_t  scal = 0;      // 比例因子，取值只能是1,2,4,8
} operand_t;

// 指令对应的处理函数
typedef void (*operator_t)(operand_t* src, operand_t* dst);

typedef struct {
    operator_t operator;
    operand_t src;
    operand_t dst;
    const char* inst; 
} inst_t;



#endif