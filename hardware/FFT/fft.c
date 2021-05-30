#include "fft.h"

// 设置虚部全部为0
void Set_inFFT_IM_0(float *inSignal, uint32_t fftlength)
{
    uint32_t i;
    for (i = 0; i < fftlength; i++)
    {
        inSignal[2 * i + 1] = 0; //虚部全部为0
    }
}
// 设置实部全部为0
void Set_inFFT_RE_0(float *inSignal, uint32_t fftlength)
{
    uint32_t i;

    for (i = 0; i < fftlength; i++)
    {
        inSignal[2 * i] = 0;
    }
}

// 设置实部虚部全部为0
void Set_inFFT_RE_IM_0(float *inSignal, uint32_t fftlength)
{
    uint32_t i;
    for (i = 0; i < (fftlength * 2); i++)
    {
        inSignal[i] = 0;
    }
}

void Set_inFFT_RE_DC(float *inSignal, uint32_t fftlength, uint16_t dclevel)
{
    uint32_t i;

    for (i = 0; i < fftlength; i++)
    {
        inSignal[2 * i] += dclevel;
    }
}

// 生成正弦信号
void Set_inFFT_RE_TO_Sin(float *inSignal, uint16_t am, uint32_t freq, uint32_t fftlength, uint32_t samplefreq)
{ //  存放的数组        振幅         频率           采样深度              采样频率
    uint32_t i;
    for (i = 0; i < fftlength; i++)
    {
        inSignal[2 * i] += am * arm_sin_f32(2 * PI * i * freq / samplefreq);
    }
}

// 生成余弦信号
void Set_inFFT_RE_TO_Cos(float *inSignal, uint16_t am, uint32_t freq, uint32_t fftlength, uint32_t samplefreq)
{ //  存放的数组        振幅         频率           采样深度              采样频率
    uint32_t i;
    for (i = 0; i < fftlength; i++)
    {
        inSignal[2 * i] += am * arm_cos_f32(2 * PI * i * freq / samplefreq); // samplefreq/freq: 表示该频率的信号占多少样点/周期
    }
}
// 产生合成的正弦波
void Generate_Wave_Sin(float *inSignal, uint32_t fftlength, uint32_t samplefreq)
{
    uint32_t freq1, freq2, freq3, freq4, freq5; // 频率HZ
    uint16_t am1, am2, am3, am4, am5;           // 振幅 无单位 [1-2047]

    am1 = 150; //
    freq1 = 6;
    am2 = 300;
    freq2 = 77;
    am3 = 500;
    freq3 = 690;
    am4 = 1000;
    freq4 = 1000;
    am5 = 800;
    freq5 = 1700;

    Set_inFFT_RE_IM_0(inSignal, fftlength);     // 将实部虚部清0
    Set_inFFT_RE_DC(inSignal, fftlength, 2000); // 设置直流电平
    Set_inFFT_RE_TO_Sin(inSignal, am1, freq1, fftlength, samplefreq);
    Set_inFFT_RE_TO_Sin(inSignal, am2, freq2, fftlength, samplefreq);
    Set_inFFT_RE_TO_Sin(inSignal, am3, freq3, fftlength, samplefreq);
    Set_inFFT_RE_TO_Sin(inSignal, am4, freq4, fftlength, samplefreq);
    //    Set_inFFT_RE_TO_Sin(inSignal, am5,  freq5, fftlength, samplefreq); // 2048Hz采样时混叠
}

// 产生脉冲波
void Generate_Wave_Pulse(float *inSignal, uint32_t fftlength, uint32_t samplefreq)
{
    uint32_t i, freq, signalH, signalL;
    uint32_t numberofdots_aPeroid, numberofdots_H, tmp1;
    float dutyratio, ftmp;

    signalH = 3000;   // 高电平电压
    signalL = 800;    // 低电平电压
    freq = 16;        // 频率HZ
    dutyratio = 12.5; // 占空比 12.5%

    Set_inFFT_RE_IM_0(inSignal, fftlength); // 将实部虚部清0

    ftmp = samplefreq;
    ftmp = ftmp / freq;                                  // samplefreq/freq: 表示该频率的信号1个周期占多少样点
    numberofdots_aPeroid = (uint32_t)ftmp;               // 该频率的信号1个周期占多少样点
    numberofdots_H = (uint32_t)(ftmp * dutyratio / 100); // 该频率的信号周期中高电平占多少样点
    for (i = 0; i < FFT_LENGTH; i++)                     //生成信号序列
    {
        tmp1 = i % numberofdots_aPeroid; // 现在该点在x周期序列中的第几个点
        if (tmp1 < numberofdots_H)       // 小于 dutyratio% 的为高电平
        {
            inSignal[2 * i] = signalH;
        }
        else
        {
            inSignal[2 * i] = signalL;
        }
    }
}

void Windows_Input_Param(float *fftinbuf, uint32_t fftlength)
{
    uint32_t i;
    float ftmp;

    for (i = 0; i < fftlength; i++)
    {
        ftmp = fftinbuf[2 * i]; // 实部
        //ftmp = ftmp * FWin_Param[i];
        fftinbuf[2 * i] = ftmp;
    }
}

void Calculate_FFT(float *insignal, float *outsignal, uint32_t fftlength)
{
    arm_cfft_radix4_instance_f32 scfft;

    arm_cfft_radix4_init_f32(&scfft, fftlength, 0, 1); //初始化scfft结构体，设定FFT相关参数
    arm_cfft_radix4_f32(&scfft, insignal);             //FFT计算（基4）
    arm_cmplx_mag_f32(insignal, outsignal, fftlength); //把运算结果复数求模得幅值
}

void Write_ADCBuf_toFFTbuf(uint16_t *adcbuf, float *fftinbuf, uint32_t adclength)
{
    uint32_t i;
    for (i = 0; i < adclength; i++)
    {
        fftinbuf[2 * i] = (float)adcbuf[i]; // 实部
        fftinbuf[2 * i + 1] = 0;            // 虚部全部为0
    }
}
