#include "cpu/cpu.h"

uint64_t operand2vaddr(operand_t operand) {
    if (operand.scal % 2 != 0 || operand.scal > 8) {
        printf("operand error! operand scal isn't 0,1,2,4,8.
                operand scal = %d\n", operand.scal);
        exit(1);
    }
    switch(operand.op_type) {
    case REG:
        return operand.reg1;
    case MEM_IMM:
        return operand.imm;
    case MEM_IMM_REG:
        return operand.imm + operand.reg1;
    case MEM_REG_REG:
        return operand.reg1 + operand.reg2;
    case MEM_IMM_REG_REG:
        return operand.imm + operand.reg1 + operand.reg2;
    case MEM_REG_SCAL:
        return operand.reg2 * operand.scal;
    case MEM_IMM_REG_SCAL:
        return operand.imm + operand.reg2 * operand.scal;
    case MEM_REG_REG_SCAL:
        return operand.reg1 + operand.reg2 * operand.scal;
    case MEM_IMM_REG_REG_SCAL:
        return operand.imm + operand.reg1 + operand.reg2 * operand.scal;
    default:
        printf("decode operand error! operand_type_id = %d\n", operand.op_type);
        exit(1);
    }
}

void instruction_cycle(inst_t* program, uint64_t inst_num) {

    for (int i = 0; i < inst_num; i++) {
        // 取值阶段

        // 指令译码阶段

        // 指令执行阶段
    }
     
}