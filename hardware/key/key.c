#include "key.h"
#include "fft.h"
#include "tftDisp.h"
#include "led.h"

void IO_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    KEY_GPIO_CLK_ENABLE();

    // LED1的引脚初始化
    GPIO_InitStruct.Pin = IO1_pin | IO2_pin | IO3_pin | IO4_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOx_IO_PORT, &GPIO_InitStruct);
    HAL_GPIO_TogglePin(GPIOx_IO_PORT, IO2_pin);
}

/************************************************************************
// 函 数 名:      void  Key_Init(void)
// 函数功能描述:  完成六个按键端口及引脚的初始化                
// 参数描述:      无
// 返 回 值:      无
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     完成测试
// 函数层次：     面向按键电路的--硬件接口层

// 创 建 人：     zhaoweiliang
// 日期：         2014-12-14
// 修 改 人：                                                                         
// 修改日期：     201x-xx-xx
************************************************************************/
void Key_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    KEY_GPIO_CLK_ENABLE();

    // LED1的引脚初始化
    GPIO_InitStruct.Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOx_KEY_PORT, &GPIO_InitStruct);
}

/************************************************************************
// 函 数 名:      Init_KeyStatus(struKeyStatus *key)
// 函数功能描述:  初始化按键的状态               
// 参数描述:      struKeysStatus  *key:  
// 返 回 值:      无
// 返回值描述:    无
// 全局变量:      无
// 静态变量:      key1_last,key2_last,key3_last,key4_last: 
// 函数状态：     完成测试
// 函数层次：     面向按键电路的--软件驱动层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
************************************************************************/
void Init_KeyStatus(struKeyStatus *key4)
{
    key4->key1 = KEY_NO_PRESSED;
    key4->key2 = KEY_NO_PRESSED;
    key4->key3 = KEY_NO_PRESSED;
    key4->key4 = KEY_NO_PRESSED;
}

/************************************************************************
// 函 数 名:      void  Read_KeyStatus(struKeysStatus  *keynow)
// 函数功能描述:  读取按键当前的状态                
// 参数描述:      struKeysStatus  *keynow:  保存4个按键当前的状态 
//                保存上一次4个按键的状态，通过和现在的按键状态比较，获得当前各个按键的状态和计数值
// 返 回 值:      无
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     完成测试
// 函数层次：     面向按键电路的--硬件接口层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
************************************************************************/
void Read_KeyStatus(struKeyStatus *keynow)
{
    KeyStatus key_last;

    /////////////////////////////////////////////////////////////
    // 获得KEY1的状态
    key_last = keynow->key1; // 保存上次的KEY1的状态
    if (!READ_KEY1())        // KEY1的电平按下为0, 弹出为1;
    {                        // KEY1当前的电平为按下
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key1 = KEY_ALREADY_PRESSED;
        }
        else
        {
            keynow->key1 = KEY_JUST_PRESSED;
        }
    }
    else // KEY1当前的电平为弹出
    {
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key1 = KEY_JUST_POPUP;
        }
        else
        {
            keynow->key1 = KEY_NO_PRESSED;
        }
    }
    /////////////////////////////////////////////////////////////
    // 获得KEY2的状态
    key_last = keynow->key2; // 保存上次的KEY1的状态
    if (!READ_KEY2())        // KEY2的电平按下为0, 弹出为1;
    {                        // KEY2当前的电平为按下
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key2 = KEY_ALREADY_PRESSED;
        }
        else
        {
            keynow->key2 = KEY_JUST_PRESSED;
        }
    }
    else // KEY2当前的电平为弹出
    {
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key2 = KEY_JUST_POPUP;
        }
        else
        {
            keynow->key2 = KEY_NO_PRESSED;
        }
    }
    /////////////////////////////////////////////////////////////
    // 获得KEY3的状态
    key_last = keynow->key3; // 保存上次的KEY3的状态
    if (!READ_KEY3())        // KEY3的电平按下为0, 弹出为1;
    {                        // KEY3当前的电平为按下
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key3 = KEY_ALREADY_PRESSED;
        }
        else
        {
            keynow->key3 = KEY_JUST_PRESSED;
        }
    }
    else // KEY3当前的电平为弹出
    {
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key3 = KEY_JUST_POPUP;
        }
        else
        {
            keynow->key3 = KEY_NO_PRESSED;
        }
    }
    /////////////////////////////////////////////////////////////
    // 获得KEY4的状态
    key_last = keynow->key4; // 保存上次的KEY4的状态
    if (!READ_KEY4())        // KEY4的电平按下为0, 弹出为1;
    {                        // KEY4当前的电平为按下
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key4 = KEY_ALREADY_PRESSED;
        }
        else
        {
            keynow->key4 = KEY_JUST_PRESSED;
        }
    }
    else // KEY4当前的电平为弹出
    {
        if ((key_last == KEY_JUST_PRESSED) || (key_last == KEY_ALREADY_PRESSED))
        {
            keynow->key4 = KEY_JUST_POPUP;
        }
        else
        {
            keynow->key4 = KEY_NO_PRESSED;
        }
    }
}

/************************************************************************
// 函 数 名:      void  Key_Process(struKeyStatus *key)
// 函数功能描述:  按键的处理                
// 参数描述:      struKeysStatus  *key:  4个按键当前的状态
// 返 回 值:      无
// 返回值描述:    无
// 全局变量:      无
// 静态变量:      无
// 函数状态：     完成测试
// 函数层次：     应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
************************************************************************/
void Key_Process(struKeyStatus key, uint16_t *x_freq, FunctionalState *Dispx_en)
{
    if (key.key1 == KEY_JUST_PRESSED) // 按键1 刚按下时的动作
    {
        HAL_GPIO_TogglePin(GPIOx_IO_PORT, IO1_pin);
        LED1_TOGGLE();
    }

    if (key.key2 == KEY_JUST_PRESSED) // 按键2 刚按下时的动作
    {
        HAL_GPIO_TogglePin(GPIOx_IO_PORT, IO2_pin);
        LED2_TOGGLE();
    }

    if (key.key3 == KEY_JUST_PRESSED) // 按键3 刚按下时的动作
    {
        HAL_GPIO_TogglePin(GPIOx_IO_PORT, IO3_pin);
        LED3_TOGGLE();
    }

    if (key.key4 == KEY_JUST_PRESSED) // 按键4 刚按下时的动作
    {
        HAL_GPIO_TogglePin(GPIOx_IO_PORT, IO4_pin);
        LED4_TOGGLE();
    }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
