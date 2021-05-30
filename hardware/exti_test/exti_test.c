#include  "exti_test.h"

void  Exti_Test_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*Configure GPIO pin : PG8 */
    EXTI_TEST_IN_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = EXTI_TEST_IN_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING_FALLING;   // 电平触发
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(EXTI_TEST_IN_PORT, &GPIO_InitStruct);    // 此处初始化包含了中断的初始化
         
    /*Configure GPIO pin : PF0 */
    EXTI_TEST_OUT_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin   = EXTI_TEST_OUT_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(EXTI_TEST_OUT_PORT, &GPIO_InitStruct);   // PF0用于跟踪PG8
}

//////////////////
// 外部引脚中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == EXTI_TEST_IN_PIN)   // 外部引脚触发中断的回调函数
  {
      if(Get_EXTI_TEST_Pin())        // 中断引脚高电平, 跟踪引脚输出高电平
      {
          EXTI_TEST_OUT_H();
      }
      else
      {
          EXTI_TEST_OUT_L();         // 中断引脚低电平, 跟踪引脚输出低电平
      }
  }
}

