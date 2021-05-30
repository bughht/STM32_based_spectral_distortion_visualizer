/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "string.h"

// USART1 接收
uint16_t    U1_Rec_Point;                     // 接收指针
uint8_t     U1_Rec_Buffer[U1_REC_MAX_BYTES];  // 接收缓存区 
uint8_t     U1_Send_Buffer[U1_SEND_MAX_BYTES];// USART1  发送缓存区 
uint8_t     Rx1Buffer;                        // 直接接收的每一个字节都存放这里
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

// 设定的数据接收结束，产生回调函数
void    HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{   
    // 串口1的接收中断处理
    if(UartHandle->Instance==USART1) 
    { 
        U1_Rec_Buffer[U1_Rec_Point] = Rx1Buffer;
        U1_Rec_Point++;
        if(U1_Rec_Point >= U1_REC_MAX_BYTES) { U1_Rec_Point = 0; }
        // 接收数据的处理
        // U1_Process(U1_Rec_Buffer,U1_Rec_Point);  // 根据自己的要求去写完成
        
        HAL_UART_Receive_IT(&huart1,&Rx1Buffer,1);  // 设置接收1个字节 产生中断回调 开启下一次接收中断
    }
    
    // 串口2的接收中断处理
    if(UartHandle->Instance==USART2)
    { 

    }
    // 串口3的接收中断处理
    if(UartHandle->Instance==USART3)
    { 

    }   
    // 串口6的接收中断处理
    if(UartHandle->Instance==USART6)
    { 

    }       
}

// 设定的数据发送结束，产生回调函数
void  HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{    
    if(UartHandle->Instance==USART1)  // 串口1 完成发送自己设置的字节数量后，产生发送完成中断回调
    { 
        //Switch_RS485_CPU_Rec();       // 切换RS485到接收状态
    }
    
    if(UartHandle->Instance==USART2)  // 
    { 
        
    }

    if(UartHandle->Instance==USART3)  // 
    { 
        
    }
}

// 串口1的发送测试函数
void  U1_Test_SendString(void)
{
    uint16_t  strlength;

    memset(U1_Send_Buffer, '\0', U1_SEND_MAX_BYTES);
//    strcpy(U1_Send_Buffer,"上海大学.通信学院."); // 字符串赋值 前面要加" #include "string.h" "
    strcpy((char*)U1_Send_Buffer,"OK上海大学.通信学院. OK"); //字符串赋值 前面要加" #include "string.h" " 此法可以去掉一个警告
//    strlength = strlen(U1_Send_Buffer);           // 获得字符串的长度
    strlength = strlen((char*)U1_Send_Buffer);  // 获得字符串的长度, 此法可以去掉一个警告
    
    memset(U1_Rec_Buffer,'\0', U1_REC_MAX_BYTES);
    HAL_UART_Receive_IT(&huart1,&Rx1Buffer,1);              // 设置接收1个字节，存放在 Rx1Buffer 中, 再产生接收中断
//  HAL_UART_Receive_IT(&huart1,U1_Rec_Buffer,10);          // 设置接收满了10个字节，都存放在 U1_Rec_Buffer 中, 再产生接收中断
    
    HAL_UART_Transmit_IT(&huart1,U1_Send_Buffer,strlength); // 发送strlength个字节的数据
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
