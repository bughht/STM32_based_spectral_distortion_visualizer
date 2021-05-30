#include "spin.h"
#include "fft.h"
#include "tftDisp.h"
// 旋转编码器引脚的初始化
void  Spin_GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    SPIN_GPIO_CLK_ENABLE();
    // 右侧的旋转编码器引脚初始化
    GPIO_InitStruct.Pin  = SPIN_KEY_PIN | SPIN_A_PIN | SPIN_B_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIO_SPIN_PORT, &GPIO_InitStruct);
}

// 初始化左右旋转编码器的状态
void  Init_Spin_Status(struspin *spin)
{
    spin->rotatestatus  = SPIN_NO_ROTATE;
    spin->keystatus     = KEY_NO_PRESSED;
}

////////////////////////////////////////////////////////////
// 读取 2 旋转编码器的状态
static  LEVEL  last_spinkey_level= H;           // 保存的上一次按键的电平情况
static  SpinABstatus last_spinABst=SPIN_AB_ST3; // 保存的上一次AB引脚的电平情况
void  Read_Spin(struspin *spin)
{
    LEVEL           now_spinkey_level;
    LEVEL           now_spinA_level;
    LEVEL           now_spinB_level;
    SpinABstatus    now_spinABst;
    
    // 先读取旋转编码器的各引脚的电平值
    if( READ_SPIN_KEY() ) { now_spinkey_level = H; }  // 读取旋转编码器的按键电平值
    else                  { now_spinkey_level = L; }
    
    if( READ_SPIN_A()   ) { now_spinA_level   = H; }  // 读取旋转编码器的 A 信号电平值
    else                  { now_spinA_level   = L; }
    
    if( READ_SPIN_B()   ) { now_spinB_level   = H; }  // 读取旋转编码器的 B 信号电平值
    else                  { now_spinB_level   = L; }
    
    // 根据 旋转编码器的按键 前后 两次的电平值, 确定按键状态
    if(     (last_spinkey_level == H)  && (now_spinkey_level == L) )  { spin->keystatus  = KEY_JUST_PRESSED;    } 
    else if((last_spinkey_level == H)  && (now_spinkey_level == H) )  { spin->keystatus  = KEY_NO_PRESSED;      } 
    else if((last_spinkey_level == L)  && (now_spinkey_level == H) )  { spin->keystatus  = KEY_JUST_POPUP;      } 
    else                                                              { spin->keystatus  = KEY_ALREADY_PRESSED; } 

    //  根据 左侧旋转编码器的 A B 的电平值, 确定 AB 状态
    if(     (now_spinA_level == H)  && (now_spinB_level == H) )  { now_spinABst  = SPIN_AB_ST3; }  // A=1 B=1; 
    else if((now_spinA_level == H)  && (now_spinB_level == L) )  { now_spinABst  = SPIN_AB_ST2; }  // A=1 B=0;
    else if((now_spinA_level == L)  && (now_spinB_level == L) )  { now_spinABst  = SPIN_AB_ST0; }  // A=0 B=0;
    else                                                         { now_spinABst  = SPIN_AB_ST1; }  // A=0 B=1;
   
    // 再根据 左侧旋转编码器 前后两次 AB信号线的状态, 确定是左旋还是右旋
    if(      (last_spinABst == SPIN_AB_ST3) && (now_spinABst == SPIN_AB_ST2)){ spin->rotatestatus = SPIN_CLOCKWISE;     }
    else if ((last_spinABst == SPIN_AB_ST3) && (now_spinABst == SPIN_AB_ST1)){ spin->rotatestatus = SPIN_UNTICLOCKWISE; }
    else                                                                     { spin->rotatestatus = SPIN_NO_ROTATE;     }
  
    last_spinkey_level  = now_spinkey_level;  // 保存当前状态，作为下次状态的依据
    last_spinABst       = now_spinABst;
}

void  Proc_Spin(struspin spin, uint16_t *x_freq, FunctionalState *Dispx_en)
{
    if(spin.rotatestatus == SPIN_CLOCKWISE)
    {
        if((*x_freq) >= (FFT_LENGTH / 2 - FFT_SUBWIN_SPECTRUM_MAX / 2 ))
        {
            *x_freq = 0;
        }
        else
        {
            (*x_freq)++;
        }
        *Dispx_en = ENABLE;
    }
    else  if(spin.rotatestatus == SPIN_UNTICLOCKWISE)
    {
        if((*x_freq) == 0)
        {
            *x_freq = FFT_LENGTH / 2 - FFT_SUBWIN_SPECTRUM_MAX / 2;
        }
        else
        {
            (*x_freq)--;
        }
        *Dispx_en = ENABLE;
    }
}

