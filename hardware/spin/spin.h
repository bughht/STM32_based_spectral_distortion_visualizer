#ifndef  __SPIN_H
#define  __SPIN_H

#include  "stm32f4xx_hal.h"
#include  "key.h"
//////////////////////////////////////////////////////

#define    SPIN_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE() //  打开右面的旋转编码器使用的GPIOE时钟
#define    GPIO_SPIN_PORT              GPIOE                 //  右面的旋转编码器驱动引脚所在的端口
#define    SPIN_KEY_PIN                GPIO_PIN_6            // 右面的旋转编码器 按键 引脚号
#define    SPIN_A_PIN                  GPIO_PIN_4            // 右面的旋转编码器 A 引脚号
#define    SPIN_B_PIN                  GPIO_PIN_5            // 右面的旋转编码器 B 引脚号

#define    READ_SPIN_KEY()             HAL_GPIO_ReadPin(GPIO_SPIN_PORT, SPIN_KEY_PIN) // PE0
#define    READ_SPIN_A()               HAL_GPIO_ReadPin(GPIO_SPIN_PORT, SPIN_A_PIN)   // PE2 读入 A 引脚的电平
#define    READ_SPIN_B()               HAL_GPIO_ReadPin(GPIO_SPIN_PORT, SPIN_B_PIN)   // PE1 读入 B 引脚的电平


///////////////////////////////////////////////////
typedef enum
{
    L = 0,    // 低电平
    H    ,    // 高电平
}LEVEL;

//typedef  enum
//{
//    KEY_NO_PRESSED      = 0,
//    KEY_JUST_PRESSED       ,
//    KEY_ALREADY_PRESSED    ,
//    KEY_JUST_POPUP         ,
//}KeyStatus;

typedef enum
{
    SPIN_NO_ROTATE   = 0,  // 无旋转
    SPIN_CLOCKWISE      ,  // 顺时针旋转
    SPIN_UNTICLOCKWISE  ,  // 反时针旋转
}RotateStatus;

typedef struct tagspin
{
    RotateStatus   rotatestatus; // 旋转状态
    KeyStatus      keystatus;    // 按键状态
}struspin;

typedef enum
{
    SPIN_AB_ST0  = 0,    // A=0 B=0;
    SPIN_AB_ST1     ,    // A=0 B=1;
    SPIN_AB_ST2     ,    // A=1 B=0;
    SPIN_AB_ST3     ,    // A=1 B=1; 
}SpinABstatus;          // 旋转编码器信号线 A B 的电平



///////////////////////////////////////////////////
void  Spin_GpioInit(void);
void  Init_Spin_Status(struspin *);  // 初始化左右旋转编码器的状态
void  Read_Spin(struspin *);
void  Proc_Spin(struspin , uint16_t *, FunctionalState *);
    
#endif


