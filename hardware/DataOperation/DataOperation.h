/*
 * @Author: Bughht
 * @Date: 2021-05-19 16:38:01
 * @LastEditTime: 2021-05-23 21:54:30
 * @Description: This Library contains data operations on adc buffer, fft buffer and virtual waveform generation.
 * Waring: Something extremely weired happened when data operations on fft_result, may lead to unexpected results, never try to change it! This is magic!
 * @FilePath: \MDK-ARMe:\SHU\GCJY\BIG\hardware\DataOperation\DataOperation.h
 */
#ifndef __DATAOPERATION_H__
#define __DATAOPERATION_H__

#include "adc.h"
#include "fft.h"

extern float ADC_Voltage_buf[SIZEofADC1];
extern float fft_buf[FFT_LENGTH * 2];
extern float fft_Result[FFT_LENGTH * 2];
extern float fft_dB[FFT_LENGTH * 2];

void GenerateSinData(float freq);
void GenerateSquareData(float freq);
void GenerateFullWaveData(float freq);
void ADCBuf_Init(void);
void ADCBuf_Normalize(void);
void AC_Coupling(void);
void ADC2FFTbuffer(void);
float Hamming(float f, int i, int N, int a0);
float THD_Calculation(void);

#endif
