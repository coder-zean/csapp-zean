#ifndef REGISTER_GUARD
#define REGISTER_GUARD

#include <stdint.h>

// 通用寄存器
typedef struct {
#define XX(name_64, name_32, name_16, name_8l, name_8h) \
  union {                                     \
    uint64_t name_64;                         \
    uint32_t name_32;                         \
    uint16_t name_16;                         \
    struct {                                  \
      uint8_t name_8l;                        \
      uint8_t name_8h;                        \
    };                                        \
  };
  XX(rax, eax, ax, al, ah);
  XX(rbx, ebx, bx, bl, bh);
  XX(rcx, ecx, cx, cl, ch);
  XX(rdx, edx, dx, dl, dh);
  XX(rsi, esi, si, sil, sih);
  XX(rdi, edi, di, dil, dih);
  XX(rbp, ebp, bp, bpl, bph);
  XX(rsp, esp, sp, spl, sph);
#undef XX

#define XX(name_64, name_32, name_16, name_8) \
  union {                                     \
    uint64_t name_64;                         \
    uint32_t name_32;                         \
    uint16_t name_16;                         \
    uint8_t  name_8;                          \
  };
  XX(r8, r8d, r8w, r8b);
  XX(r9, r9d, r9w, r9b);
  XX(r10, r10d, r10w, r10b);
  XX(r11, r11d, r11w, r11b);
  XX(r12, r12d, r12w, r12b);
  XX(r13, r13d, r13w, r13b);
  XX(r14, r14d, r14w, r14b);
  XX(r15, r15d, r15w, r15b);
#undef XX
} cpu_general_reg_t;

cpu_general_reg_t cpu_general_reg;

// 状态寄存器
typedef struct {
  uint64_t eflags;
  bool ZF;
  bool CF;
  bool SF;
  bool OF;
} cpu_state_reg_t;

cpu_state_reg_t cpu_state_reg;

// 程序计数器PC
typedef union {
  uint64_t rip;
  uint32_t eip;
} PC_t;

PC_t PC;

#endif