
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "key.h"
#include "spin.h"
#include "led.h"
#include "common.h"
#include "exti_test.h"
#include "lcd_drive.h"
#include "gui.h"
#include "tftDisp.h"
#include "w25q64.h"
#include "touch.h"
#include "fft.h"
#include "windowsframe.h"
#include "DataOperation.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// 例程：FFT
// Tim2.捕获比较通道2的上升沿 触发一路 ADC1_IN10 进行ADC转换，完成后自动通过DMA通道存在数组 ADC1_Value[]中。
//      当存满后，触发 "DMA2_Stream0_IRQHandler" 中断， 调用"ADC_DMAConvCplt",再调用"HAL_ADC_ConvCpltCallback" 进行数据处理

// 显示必须按转成 dB 单位, 主窗口显示整个频谱，子窗口显示部分详细谱线，主窗口下方的小箭头指示了子窗口频谱在主窗口中的位置
// 1、可以按键K3K4调节显示子窗口的频谱
// 2、可以用旋转编码器细调子窗口的频谱
// 3、可以点触主窗口区域来初调子窗口的频谱
// 4、可以滑动子窗口区域来  调子窗口的频谱

/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

// 定时器处理溢出中断，中断频率理论可以很高，但实际由于进入中断要一些时间，一般最高500KHz;
// 定时器设置时中断频率的计算方法: Fclk/(Prescaler+1)/(Period)
// Fclk:定时器所在总线频率*2;  Prescaler:预分频值;  Period:向上计数的溢出值
// TIMER4中断频率: 84M/(8399+1)/(999+1)=10Hz
// TIMER5中断频率: 84M/(83+1)/(999999+1)=1Hz
// TIMER7中断频率: 84M/(83+1)/(999+1)=1000Hz
// TIMER6中断频率: 84M/(3+1)/(4+1)=4.2MHz, 用于触发DAC1; 当DAC1定时触发时,只能用指定的TIM6来完成,TIM6可以不设为中断模式

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    struKeyStatus key; // 四个按键的状态变量
    struTouch ctpxy;   // 电容触摸屏的参数
    struspin spin;

    //float fft_inputbuf[FFT_LENGTH * 2]; //FFT输入数组
    //float fft_outputbuf[FFT_LENGTH];    //FFT输出数组

    uint16_t disp_x_freq_left;      // 详细的9根谱线最左边的点数
    FunctionalState disp_x_freq_en; // 显示详细频谱使能
    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();
    MX_SPI2_Init();
    MX_TIM5_Init();
    MX_TIM7_Init();
    MX_FSMC_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();
    /* USER CODE BEGIN 2 */
    //_TIM2_SetFreq();

    HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1); // 使能TIM4计数，PA1输出PWM信号

    HAL_TIM_Base_Start_IT(&htim7);

    W25Q64_CS_GPIO_Init();  // W25Q64.CS引脚初始化
    FT6336_I2C_GPIO_Init(); // LCD屏的触摸屏的2个引脚初始化
    LCD_Initialize();       // LCD屏的复位引脚，背光引脚 初始化

    IO_GPIO_Init();
    LED_GPIO_Init();       // 四个LED指示灯 引脚初始化 （推挽输出）
    Key_GPIO_Init();       // 四个按键      引脚初始化 （上拉输入）
    Exti_Test_GPIO_Init(); // 中断输入引脚的初始化, 及测试中端的IO引进的初始化

    Spin_GpioInit(); // 旋转编码器的IO引脚初始化
    spin.keystatus = KEY_NO_PRESSED;
    spin.rotatestatus = SPIN_NO_ROTATE;
    //////////////////////
    Init_KeyStatus(&key); // 四个按键 状态初始化为 "没有按下键"

    ProcADC_Enable = DISABLE; // 不允许ADC1_IN0变换
    // ADC TIM2的CC2匹配事件触发
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_Value, SIZEofADC1); // 设置ADC1_IN0的DMA 数据缓存器
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);                      // 使能TIM2计数，PA1输出PWM信号

    ADCBuf_Init();
    ADCBuf_Normalize();

    t_ratio = 50;
    f_ratio = 2;

    Lcd_Clear_All(BLACK);

    Voltage_time_fig = Figure_init(Point(64, 30), Point(355, 350));
    sprintf(Voltage_time_fig.xlabel, "t/ms");
    sprintf(Voltage_time_fig.ylabel, "U/Voltage");
    sprintf(Voltage_time_fig.title, "时域");
    Figure_bound_data(&Voltage_time_fig, ADC_Voltage_buf + 10, (float)SIZEofADC1 / t_ratio);
    Figure_window_plot(&Voltage_time_fig);
    Voltage_time_fig.xrange[0] = 0;
    Voltage_time_fig.xrange[1] = (float)125 / t_ratio;
    Voltage_time_fig.xlegendnum = 9;
    Voltage_time_fig.ylegendnum = 21;

    FFT_fig = Figure_init(Point(420, 30), Point(X_MAX_PIXEL - 15, 350));
    sprintf(FFT_fig.xlabel, "f/kHz");
    sprintf(FFT_fig.ylabel, "U/Voltage");
    sprintf(FFT_fig.title, "频域");
    Figure_bound_data(&FFT_fig, fft_Result, FFT_LENGTH / f_ratio);
    Figure_window_plot(&FFT_fig);
    FFT_fig.xrange[0] = 0;
    FFT_fig.xrange[1] = (SAMPLE_FREQ / 1000) / f_ratio;
    FFT_fig.xlegendnum = 13;
    FFT_fig.ylegendnum = 21;

    StopButton = Button_init(Point(700, 400), Point(800, 450), " Pause", " Resume", WHITE_4_4, WHITE_1_4, RED_4_4);
    Button_plot(&StopButton);

    /////////////////////////
    //Init_Last_Spectrum(Last_Spectrum); // 初始化主谱线的前值
    //Inin_Win();                        // 初始化LCD显示的不变的内容
    //disp_x_freq_left = 0;              // 子窗口最左侧频率=0
    //disp_x_freq_en = ENABLE; // 开机使能显示

    /////////////////////////
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /*
        if (disp_x_freq_en == ENABLE)
        { // 用旋转编码器 或 K3 K4 或 触屏 改变 详细频谱的最左侧的频率值，向右则依次+0.5HZ
            disp_x_freq_en = DISABLE;
            DRAW_MainWin_Arrow_Freq(disp_x_freq_left);                 // 画出主谱线下方的小箭头，指向子窗口详细谱线在主谱线中开始的位置
            Write_SubWin_Left_Center_Freq(disp_x_freq_left);           // 在子窗口在详细谱线图中标出第一根和中间和最后1根谱线的频率值
            Draw_SubWin_FFT_Spectrum(disp_x_freq_left, fft_outputbuf); // 在子窗口画出详细谱线图
        }
        */
        // 触摸屏的处理
        if (ReadTouch_Enable == ENABLE) // 每10mS允许读取按键和触摸屏, 并且处理
        {
            ReadTouch_Enable = DISABLE;
            ReadCTP(&ctpxy); // 读取触摸屏的触摸坐标及状态
            if (ctpxy.ctpmainstatus == JUST_TOUCH)
            {
                if (Button_Checkpos(&StopButton, ctpxy.ctpxy.ctp_x, ctpxy.ctpxy.ctp_y) == TRUE)
                {
                    StopButton.ButtonDOWN = !StopButton.ButtonDOWN;
                    Button_plot(&StopButton);
                }
            }
            if (ctpxy.ctpmainstatus == TOUCHED)
            {
                if (Figure_Checkpos(&Voltage_time_fig, ctpxy.ctpxy.ctp_x, ctpxy.ctpxy.ctp_y) == TRUE && StopButton.ButtonDOWN == TRUE)
                {
                    if (ctpxy.xmove == LEFT_MOVING)
                    {
                        t_ratio -= 2;
                        t_ratio = t_ratio < 2 ? 2 : t_ratio;
                        Voltage_time_fig.datalen = SIZEofADC1 / t_ratio;
                        Voltage_time_fig.xrange[1] = 125 / t_ratio;
                        Figure_line_plot(&Voltage_time_fig, 1.8, GREEN_4_4);
                    }
                    if (ctpxy.xmove == RIGHT_MOVING)
                    {
                        t_ratio += 2;
                        t_ratio = t_ratio > 100 ? 100 : t_ratio;
                        Voltage_time_fig.datalen = SIZEofADC1 / t_ratio;
                        Voltage_time_fig.xrange[1] = 125 / t_ratio;
                        Figure_line_plot(&Voltage_time_fig, 1.8, GREEN_4_4);
                    }
                }
                if (Figure_Checkpos(&FFT_fig, ctpxy.ctpxy.ctp_x, ctpxy.ctpxy.ctp_y) == TRUE && StopButton.ButtonDOWN == TRUE)
                {
                    if (ctpxy.xmove == LEFT_MOVING)
                    {
                        f_ratio -= 0.4;
                        //f_ratio += ctpxy.xmove / 100;
                        f_ratio = f_ratio < 2 ? 2 : f_ratio;
                        FFT_fig.datalen = FFT_LENGTH / f_ratio;
                        FFT_fig.xrange[1] = SAMPLE_FREQ / 1000.0 / f_ratio;
                        Figure_line_plot(&FFT_fig, 1, PINGKISH_4_4);
                    }
                    if (ctpxy.xmove == RIGHT_MOVING)
                    {
                        f_ratio += 0.4;
                        f_ratio = f_ratio > 30 ? 30 : f_ratio;
                        FFT_fig.datalen = FFT_LENGTH / f_ratio;
                        FFT_fig.xrange[1] = SAMPLE_FREQ / 1000.0 / f_ratio;
                        Figure_line_plot(&FFT_fig, 1, PINGKISH_4_4);
                    }
                }
            }
            //Proc_Touch_inMainWin(ctpxy, &disp_x_freq_left, &disp_x_freq_en); // 根据主窗口的触摸情况，改变在子窗口详细谱线图中第一根谱线的频率值，细调
            //Proc_Touch_inSubWin(ctpxy, &disp_x_freq_left, &disp_x_freq_en);  // 根据子窗口的触摸情况，改变在子窗口详细谱线图中第一根谱线的频率值，细调
        }
        // 按键和旋转编码器的处理
        if (ReadKey_Enable == ENABLE)
        {
            ReadKey_Enable = DISABLE;
            ////////////////////////

            Read_KeyStatus(&key); // 读取四个按键的状态，包含了去抖动
            Key_Process(key, &disp_x_freq_left, &disp_x_freq_en); // 根据K3 K4按键的状态，改变在详细谱线图中第一根谱线的频率值，粗调
            ////////////////////////
            /*
            Read_Spin(&spin);                                    // 读取旋转编码器的值
            Proc_Spin(spin, &disp_x_freq_left, &disp_x_freq_en); // 根据转动情况，改变在详细谱线图中第一根谱线的频率值，细调
            */
        }

        // 完成一次采样周期后，数据的处理
        if (ProcADC_Enable == ENABLE) // 完成一组ADC转换后，该标志位 被使能
        {
            //TIM2->DIER &= 0;
            //TIM2->CR1 &= ~(1);
            HAL_TIM_Base_Stop_IT(&htim2);
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
            HAL_TIM_Base_Stop(&htim2);
            //LED1_TOGGLE();
            Figure_line_plot(&Voltage_time_fig, 1.8, GREEN_4_4);
            ADC2FFTbuffer();
            FFT_Cal();
            Figure_line_plot(&FFT_fig, 1, PINGKISH_4_4);

            //            Generate_Wave_Pulse(fft_inputbuf, FFT_LENGTH, SAMPLE_FREQ); // 制造一个脉冲波，频率/占空比/幅度 可调
            /*
            //Generate_Wave_Sin(fft_inputbuf, FFT_LENGTH, SAMPLE_FREQ);   // 制造多个理想正弦波，频率/幅度 可调
            Write_ADCBuf_toFFTbuf(ADC1_Value, fft_inputbuf, SIZEofADC1); // 将ADC的值写入FFT输入数组，并初始化入FFT输入数组
            Windows_Input_Param(fft_inputbuf, FFT_LENGTH);               // 对原始信号加窗，改善频谱
            Calculate_FFT(fft_inputbuf, fft_outputbuf, FFT_LENGTH);      // 计算FFT值
            // 以下为显示主谱线和详细谱线
            //Draw_xdB_Line();                                           // 显示横向  dB定标线
            //Draw_MainWin_Spectrum_Flag_Line();                         // 主窗口显示纵向频率定标线
            //Draw_MainWin_Spectrum(fft_outputbuf, FFT_LENGTH);          // 主窗口显示总体主频谱
            //Draw_SubWin_FFT_Spectrum(disp_x_freq_left, fft_outputbuf); // 在子窗口左侧频率向右显示9根详细谱线 ，0.5HZ间隔
            */
            //HAL_TIM_Base_Start_IT(&htim2);
            //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
            ProcADC_Enable = DISABLE;
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
    /* USART1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* TIM5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    /* TIM7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM7_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    /* EXTI9_5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    /* DMA2_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    /* TIM4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM4_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
