#ifndef  __COMMON_H__
#define  __COMMON_H__

#include "stm32f4xx_hal.h"

////////////////////////////////////////
// 将接收到的按IEEE741存储的float型的数转换为真实的浮点数
typedef union{
    float     f;
    uint32_t  uint32;
}U32toFloat;
////////////////////////////////////////
typedef  enum{
    FALSE = 0,
    TRUE  = !FALSE,
}BOOL;


////////////////////////////////////////
// 一个字节中的未定义
typedef struct {
    uint8_t  b0:1;
    uint8_t  b1:1;
    uint8_t  b2:1;
    uint8_t  b3:1;
    uint8_t  b4:1;
    uint8_t  b5:1;
    uint8_t  b6:1;
    uint8_t  b7:1;
}bitDef;

typedef  union{
    bitDef   bits8;
    uint8_t  abyte;
}uniBit8Byte;



///////////////////////////////////////////
void  Delay_ms(uint32_t );
void  Delay(volatile uint32_t );

uint16_t  Digital4_BCD4(uint16_t );
uint32_t  Digital6_BCD6(uint32_t );
uint32_t  Digital8_BCD8(uint32_t );

uint8_t  Swap_bit7_bit0(uint8_t );

#endif

