#ifndef  __EXTI_TEST_H__
#define  __EXTI_TEST_H__

#include "stm32f4xx_hal.h"

#define  EXTI_TEST_IN_PIN                   GPIO_PIN_8
#define  EXTI_TEST_IN_PORT                  GPIOG
#define  EXTI_TEST_IN_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define  Get_EXTI_TEST_Pin()                HAL_GPIO_ReadPin(EXTI_TEST_IN_PORT, EXTI_TEST_IN_PIN)


#define  EXTI_TEST_OUT_PIN                  GPIO_PIN_0
#define  EXTI_TEST_OUT_PORT                 GPIOF
#define  EXTI_TEST_OUT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define  EXTI_TEST_OUT_H()                  HAL_GPIO_WritePin(EXTI_TEST_OUT_PORT, EXTI_TEST_OUT_PIN, GPIO_PIN_SET)
#define  EXTI_TEST_OUT_L()                  HAL_GPIO_WritePin(EXTI_TEST_OUT_PORT, EXTI_TEST_OUT_PIN, GPIO_PIN_RESET)
#define  EXTI_TEST_OUT_TOGGLE()             HAL_GPIO_TogglePin(EXTI_TEST_OUT_PORT, EXTI_TEST_OUT_PIN)


void  Exti_Test_GPIO_Init(void);
void  HAL_GPIO_EXTI_Callback(uint16_t );
#endif
