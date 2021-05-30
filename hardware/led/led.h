#ifndef  __LED_H__
#define  __LED_H__

#include "stm32f4xx_hal.h"

////////////////////////////////////////////////////////////
#define  LED1_PIN                       GPIO_PIN_1
#define  LED1_GPIO_PORT                 GPIOB
#define  LED1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define  LED1_ON()                      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET)
#define  LED1_OFF()                     HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET)
#define  LED1_TOGGLE()                  HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN) 

#define  LED2_PIN                       GPIO_PIN_11
#define  LED2_GPIO_PORT                 GPIOF
#define  LED2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define  LED2_ON()                      HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET)
#define  LED2_OFF()                     HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET)
#define  LED2_TOGGLE()                  HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN)

#define  LED3_PIN                       GPIO_PIN_12
#define  LED3_GPIO_PORT                 GPIOF
#define  LED3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define  LED3_ON()                      HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET)
#define  LED3_OFF()                     HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET)
#define  LED3_TOGGLE()                  HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN)

#define  LED4_PIN                       GPIO_PIN_13
#define  LED4_GPIO_PORT                 GPIOF
#define  LED4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define  LED4_ON()                      HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET)
#define  LED4_OFF()                     HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET)
#define  LED4_TOGGLE()                  HAL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN)
////////////////////////////////////////////////////////////

void  LED_GPIO_Init(void);
    
#endif

