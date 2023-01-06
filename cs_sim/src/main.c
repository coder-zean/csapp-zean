#include "/root/zean-csapp/cs_sim/include/cpu/register.h"
#include <stdint.h>
#include <stdio.h>

int main() {
  RegisterSet r;
  r.rax = 0x0011223344556677;
  printf("0x%lx\n", r.rax);
  printf("0x%x\n", r.eax);
  printf("0x%x\n", r.ax);
  printf("0x%x\n", r.al);
  printf("0x%x\n", r.ah);
  r.r8 = 0x111100001112211;
  printf("0x%lx\n", r.r8);
  printf("0x%x\n", r.r8d);
  printf("0x%x\n", r.r8w);
  printf("0x%x\n", r.r8b);
  // printf("0x%x\n", r.ah);
  return 0;
}