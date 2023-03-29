#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t uint2float(uint32_t u) {
    if (u == 0) return 0;
    
    uint32_t sign = u & 0x80000000;   // 获取符号位
    u = u & 0x7fffffff;               // 去除符号位
    int msb = -1;
    // 获得最高有效位所在的位置
    for (int i = 0; i < 32; i++) {
        if (u >> i == 1) {
            msb = i + 1;
            break;
        }
    }
    
    int32_t exp = 126 + msb;                          // 计算阶码
    uint32_t frag_mask = 0xffffffff >> (33 - msb);    // 用于获取尾数部分的掩码

    // 32位float的尾数部分占23位，加上隐式表示的1，总共可以表达24位
    // 当尾数部分小于或等于24位时，说明尾数不需要进行舍入操作。
    // 尾数部分大于24位时，需要进行舍入操作。
    // 舍入规则：
    // 多余数字是指，一个28位数保留24位，则低4位为多余数字，低位第5为为最低有效位
    // 多余数字是1001，它大于0.5，故最低位进1。
    // 多余数字是0111，它小于0.5，则直接舍掉多余数字。
    // 多余数字是1000，正好是等于0.5的特殊情况:
    //      那么此时最低有效位为0则舍掉多余位，最低有效位为1则进位1。
    if (msb <= 24) {
        uint32_t frag = (u & frag_mask) << (24 - msb);
        return sign | (exp << 23) | frag;
    } else {
        uint32_t frag = u & frag_mask;
        uint32_t discard = frag & (0xffffffff >> (32 - (msb - 24)));  // 多余数位部分
        frag >>= (msb - 24);
        uint32_t zero_point_five = 0x1 << (msb - 25);                 // 计算表示0.5的值
        if (discard > zero_point_five) {
            frag += 1;
        } else if (discard == zero_point_five) {
            if (frag & 0x1 == 1) {
                frag += 1;
            }
        }
        return sign | (exp << 23) | frag;
    }
}

int main() {
    uint32_t u = 0;
    printf("请输入要转换成浮点数的数：\n");
    scanf("%u", &u);
    uint32_t uf = uint2float(u);
    printf("转换结果为:0x%x \n", uf);
    return 0;
}