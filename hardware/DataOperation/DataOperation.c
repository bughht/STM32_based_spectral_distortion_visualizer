/*
 * @Author: Bughht 
 * @Date: 2021-05-19 16:38:31
 * @LastEditTime: 2021-05-22 12:34:36
 * @Description: This Library contains data operations on adc buffer, fft buffer and virtual waveform generation.
 * Waring: Something extremely weired happened when data operations on fft_result, may lead to unexpected results, never try to change it! This is magic!
 * Warning: Optimization of the compiler must be set to Level 3!
 * @FilePath: \MDK-ARMe:\SHU\GCJY\BIG\hardware\DataOperation\DataOperation.c
 */
#include "DataOperation.h"
//#include "fft.h"
#include "math.h"
#include "arm_math.h"
#include "lcd_drive.h"
#include <math.h>

float ADC_Voltage_buf[SIZEofADC1];
float fft_buf[FFT_LENGTH * 2];
float fft_Result[FFT_LENGTH * 2];
float fft_dB[FFT_LENGTH * 2];

/**
 * @description: Generate Virtural Sin Data in ADC buffer
 * @param {float} freq
 * @return {*}
 */
void GenerateSinData(float freq)
{
    int i;
    float x = (rand() / 1000) % 5;
    for (i = 0; i < SIZEofADC1; i++)
    {
        //ADC_Voltage_buf[i] = 2 * sin(i * (freq / (16384)) * 2 * PI + x);
        ADC_Voltage_buf[i] = 2 * sin((float)i / SAMPLE_FREQ * freq * 2 * PI + x) + 1.5;
    }
}

/**
 * @description: Gererate Virtural Square Wave data in ADC buffer
 * @param {float} freq
 * @return {*}
 */
void GenerateSquareData(float freq)
{
    int i;
    float x = (rand()) / 1000;
    for (i = 0; i < SIZEofADC1; i++)
    {
        ADC_Voltage_buf[i] = (2 * sin(i * (freq / (SAMPLE_FREQ)) * 2 * PI + x)) > 0 ? 3.3 : 0;
    }
}

/**
 * @description: Generaate Virtural Full Wave rectification data in ADC buffer
 * @param {float} freq
 * @return {*}
 */
void GenerateFullWaveData(float freq)
{
    int i;
    float x = (rand()) / 1000;
    for (i = 0; i < SIZEofADC1; i++)
    {
        ADC_Voltage_buf[i] = (2 * sin(i * (freq / (SAMPLE_FREQ)) * 2 * PI + x)) > 0 ? (2 * sin(i * (freq / (SAMPLE_FREQ)) * 2 * PI + x)) : -(2 * sin(i * (freq / (SAMPLE_FREQ)) * 2 * PI + x));
    }
}

/**
 * @description: ADC buffer initialization function
 * @param {*}
 * @return {*}
 */
void ADCBuf_Init(void)
{
    for (int i = 0; i < SIZEofADC1; i++)
        ADC_Voltage_buf[i] = 0.0f;
}

/**
 * @description: Alter the 4096 Quantification rate ADC_value into 0-3.3V Voltage and write into ADC_Voltage_buf
 * @param {*}
 * @return {*}
 */
void ADCBuf_Normalize(void)
{
    int i;
    float sum = 0;
    for (i = 0; i < SIZEofADC1; i++)
        ADC_Voltage_buf[i] = ((float)ADC1_Value[i]) * 3.3f / 4096;
}

/**
 * @description: Filter the DC components of the ADC voltage buffer
 * @param {*}
 * @return {*}
 */
void AC_Coupling(void)
{
    int i = 0;
    float sum = 0;
    for (i = 0; i < SIZEofADC1; i++)
        sum += ADC_Voltage_buf[i];
    float average = sum / SIZEofADC1;
    for (i = 0; i < SIZEofADC1; i++)
        ADC_Voltage_buf[i] -= average;
}

/**
 * @description: Write ADC buffer data into FFT buffer (take top 1024 frame of data)
 * @param {*}
 * @return {*}
 */
void ADC2FFTbuffer(void)
{
    int i;

    for (i = 0; i < FFT_LENGTH; i++)
    {
        //fft_buf[2 * i] = Hamming(ADC_Voltage_buf[i], i, FFT_LENGTH, 25.0/46); // 实部
        fft_buf[2 * i] = ADC_Voltage_buf[i];
        fft_buf[2 * i + 1] = 0;
    } // 虚部全部为0
}

/**
 * @description: Calculate Hamming Window
 * Warning: It wasn't applied to the project. 
 * @param {float} f
 * @param {int} i
 * @param {int} N
 * @param {int} a0
 * @return {*}
 */
float Hamming(float f, int i, int N, int a0)
{
    float window = a0 - (1 - a0) * cos(2 * PI * i / (N - 1));
    return f * window;
}

/**
 * @description: Calculate FFT from the buffer and arrange it into Voltage values. 
 * ********************************
 * Warning: Magic!!! Don't touch!!!
 * Warning: Something extremely weired happened when carrying out data IO on fft_Result array 
 * Warning: That's the main reason why I gave up calculating dB values
 * @param {*}
 * @return {*}
 */
void FFT_Cal(void)
{
    //Calculate_FFT(fft_buf, fft_Result, FFT_LENGTH);
    //float temp;
    int i;

    char buffer[50];
    arm_cfft_radix4_instance_f32 scfft;

    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1); //初始化scfft结构体，设定FFT相关参数
    arm_cfft_radix4_f32(&scfft, fft_buf);               //FFT计算（基4）
    arm_cmplx_mag_f32(fft_buf, fft_Result, FFT_LENGTH); //把运算结果复数求模得幅值
    for (i = 0; i < FFT_LENGTH; i++)
    {
        fft_Result[i] = fft_Result[i] * 2 / FFT_LENGTH;
        //temp = fft_Result[i];

        //fft_dB[i] = 20 * log10f(temp);
        //fft_dB[i] = fft_Result[i];
    }

    sprintf(buffer, "THD = %.4f%%   ", THD_Calculation());
    Gui_Draw_Asc24_Hzk24(30, 435, WHITE_4_4, BLACK, buffer);
    //for (i = 0; i < FFT_LENGTH; i++)
    //    fft_dB[i] = 20 * log10f(fft_Result[i]);
}

/**
 * @description: Calculate THD from the Uo collected (Assume that the frequency set to be 1kHz)
 * @param {*}
 * @return {*}
 */
float THD_Calculation(void)
{
    float Uo1 = fft_Result[(int)(1000.0 / (SAMPLE_FREQ / FFT_LENGTH))];
    float Uo2 = fft_Result[(int)(2000.0 / (SAMPLE_FREQ / FFT_LENGTH))];
    float Uo3 = fft_Result[(int)(3000.0 / (SAMPLE_FREQ / FFT_LENGTH))];
    float Uo4 = fft_Result[(int)(4000.0 / (SAMPLE_FREQ / FFT_LENGTH))];
    float Uo5 = fft_Result[(int)(5000.0 / (SAMPLE_FREQ / FFT_LENGTH))];
    char buffer[100];
    sprintf(buffer, "Uo1:%.3fV Uo2:%.3fV Uo3:%.3fV Uo4:%.3fV Uo5:%.3fV", Uo1, Uo2, Uo3, Uo4, Uo5);
    Gui_Draw_Asc24_Hzk24(30, 400, WHITE_4_4, BLACK, buffer);

    float THD = sqrt(Uo2 * Uo2 + Uo3 * Uo3 + Uo4 * Uo4 + Uo5 * Uo5) / Uo1 * 100.0;
    return THD;
}
