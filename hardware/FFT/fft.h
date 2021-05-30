/*
 * @Author: your name
 * @Date: 2021-04-28 16:23:56
 * @LastEditTime: 2021-05-23 21:53:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MDK-ARMe:\SHU\GCJY\BIG\hardware\FFT\fft.h
 */
#ifndef __FFT_H__
#define __FFT_H__

#include "stm32f4xx_hal.h"
#include "math.h"
#include "arm_math.h"

//////////////////////////////////////////////////////////////////
//#define FFT_LENGTH      4096  //FFT长度, 长度只能是4^n;  4096:5.47ms; 1024:1.22ms; 256: 280us; 64:60us;

#define FFT_LENGTH 1024
#define SAMPLE_FREQ 16384 // 取样频率HZ
//#define FFT_LENGTH 16384 // FFT长度, 长度只能是4^n;
//#define SAMPLE_FREQ 20000
//实际 FFT 计算时间 4096:6.13ms; 1024:1.43ms; 256: 310us; 64:70us;

void Generate_Wave_Sin(float *inSignal, uint32_t fftlength, uint32_t samplefreq);
void Generate_Wave_Pulse(float *inSignal, uint32_t, uint32_t);
void Calculate_FFT(float *, float *, uint32_t);
void Write_ADCBuf_toFFTbuf(uint16_t *, float *, uint32_t);
void Windows_Input_Param(float *, uint32_t);

#endif
