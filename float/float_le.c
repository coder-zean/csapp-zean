/**
 * @file float_le.c
 * @author coder-zean
 * @brief csapp家庭作业2.84 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>

unsigned f2u(float x){
    return *(unsigned*)&x;
}

int float_le(float x,float y){
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);
    unsigned sx = ux>>31;
    unsigned sy = uy>>31;
    return (sx > sy) ||                  /* x < 0 and y >= 0  */
           (!sx && !sy && ux <= uy) ||   /* x >= 0 and y >= 0 */
           (sx && sx && ux >= uy);       /* x < 0 and y < 0   */
}

int main(){
    printf("%d\n",float_le(-0,+0));
    printf("%d\n",float_le(+0,-0));
    printf("%d\n",float_le(1.1,-1.1));
    printf("%d\n",float_le(1.1,1.2));
}