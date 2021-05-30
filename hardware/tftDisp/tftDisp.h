#ifndef __TFTDISP_H__
#define __TFTDISP_H__

#include "GUI.h"
#include "fft.h"
#include "DataOperation.h"

// 显示触摸坐标的参数
#define LINE_Y0 (Y_MAX_PIXEL - 41) // 横线分割触摸坐标的显示
#define X_X0 66
#define X_Y0 (Y_MAX_PIXEL - 32)
#define X_TOUCHX (X_X0 + 24)

#define Y_X0 (X_X0 + 100)
#define Y_Y0 X_Y0
#define Y_TOUCHX (Y_X0 + 24)

#define TOUCH_DIG_FORCOLOR YELLOW_4_4
#define TOUCH_DIG_BKCOLOR BLUE_4_4

// 显示测试数值的参数
#define VALUE_X_Y0 (Y_MAX_PIXEL - 32)
#define VALUE_X_X0 400
#define VALUE_X_X1 (VALUE_X_X0 + 12 * 5)
#define VAL_X_FORCOLOR WHITE_3_4
#define VAL_X_BKCOLOR GREEN_2_4

#define VALUE_Y_X0 (VALUE_X_X1 + 12 * 6)
#define VALUE_Y_X1 (VALUE_Y_X0 + 12 * 5)

// 显示时间的参数
#define DATA_TIME_Y (Y_MAX_PIXEL - 32)
#define YEAR_X0 10
#define YEAR_X1 (YEAR_X0 + 12 * 2)
#define MONTH_X (YEAR_X1 + 12 * 3)
#define DATA_X (MONTH_X + 12 * 3)
#define HOUR_X (DATA_X + 12 * 3)
#define MINUTE_X (HOUR_X + 12 * 3)
#define SECOND_X (MINUTE_X + 12 * 3)

#define DATA_TIME_FORCOLOR CYAN_4_4
#define DATA_TIME__BKCOLOR RED_2_4

////////////////////////////////////////////////////////
// 以下是主窗口显示参数
#define FFT_MAINWIN_X0 30  // X轴坐标原点
#define FFT_MAINWIN_Y0 320 // Y轴坐标原点
#define MAINWIN_WIDTH 590  // 主窗口宽度
#define MAINWIN_HEIGH 300  // 主窗口高度

// 外边框
#define FFT_MAINWIN_X0_1 (FFT_MAINWIN_X0 - 2)
#define FFT_MAINWIN_X0_2 (FFT_MAINWIN_X0 - 3)
#define FFT_MAINWIN_Y0_1 (FFT_MAINWIN_Y0 + 1)
#define FFT_MAINWIN_Y0_2 (FFT_MAINWIN_Y0 + 2)

#define FFT_MAINWIN_X1_1 (FFT_MAINWIN_X0 + MAINWIN_WIDTH + 1)
#define FFT_MAINWIN_X1_2 (FFT_MAINWIN_X0 + MAINWIN_WIDTH + 2)
#define FFT_MAINWIN_Y1_1 (FFT_MAINWIN_Y0 - MAINWIN_HEIGH - 1)
#define FFT_MAINWIN_Y1_2 (FFT_MAINWIN_Y0 - MAINWIN_HEIGH - 2)

#define FFT_MAINWIN_Y0_WORD_UNIT (FFT_MAINWIN_Y0 + 10) // 定标时写频率单位的Y坐标

#define NUMBERofDBLINES 8
#define DOTS_20dB 40
#define FFT_MAINWIN_WORD_DB_X (FFT_MAINWIN_X0 - 28)              // 定标DB时文字的X坐标
#define FFT_MAINWIN_WORD_DB_Y20 (FFT_MAINWIN_Y0 - DOTS_20dB - 8) //

#define FFT_MAINWIN_SPECT_SUM_NUMB 7 // 每相邻7根谱线合成在一根显示
#define FFT_MAINWIN_SPECT_TOTAL_DISP ((FFT_LENGTH - 1) / FFT_MAINWIN_SPECT_SUM_NUMB + 1)

#define FFT_MAINSUBWIN_COLOR WHITE_4_4
#define FFT_MAINWIN_SPECTRUM_COLOR RED_4_4      // 主频谱线的颜色
#define FFT_MAINWIN_SPECTRUM_lastCOLOR BLUE_1_4 // 主频谱线的底色

#define FFT_MAINWIN_dBVALUE_nDISP 40 // 一般20LOG10X, 40是为了显示的长度扩张一倍

#define FFT_MAINWIN_X_SPECTRUM_INTERNAL 100            // 定标频率值的间隔
#define FFT_MAINWIN_Y_FREQ_WORDS (FFT_MAINWIN_Y0 + 20) // 定标频率文字的Y坐标

#define FFT_MAINWIN_Y_ARROW (FFT_MAINWIN_Y0 + 10) // 箭头
#define FFT_MAINWIN_ARROW_FORCOLOR YELLOW_4_4
#define FFT_MAINWIN_ARROW_BKCOLOR BLUE_1_4

////////////////////////////////////////////////////////
// 以下为子窗口参数
#define FFT_SUBWIN_SPECTRUM_MAX 41 // 详细谱线画出9根
#define FFT_SUBWIN_SPEC_INTERNAL 5 // 每根间隔 5 点

// 子窗口详细谱线的外边框坐标
#define FFT_SUBWIN_X0_1 (FFT_MAINWIN_X1_1 + 7) // 里面的框的 左下角X坐标
#define FFT_SUBWIN_X0_2 (FFT_MAINWIN_X1_1 + 6) // 外面的框的 左下角X坐标
#define FFT_SUBWIN_Y0_1 FFT_MAINWIN_Y0_1       // 里面的框的 左下角Y坐标
#define FFT_SUBWIN_Y0_2 FFT_MAINWIN_Y0_2       // 外面的框的 左下角Y坐标

#define FFT_SUBWIN_X1_1 (FFT_SUBWIN_X0_1 + FFT_SUBWIN_SPECTRUM_MAX * FFT_SUBWIN_SPEC_INTERNAL + 15)
#define FFT_SUBWIN_X1_2 (FFT_SUBWIN_X1_1 + 1) // 右上
#define FFT_SUBWIN_Y1_1 FFT_MAINWIN_Y1_1
#define FFT_SUBWIN_Y1_2 FFT_MAINWIN_Y1_2

#define FFT_SUBWIN_X_ST (FFT_SUBWIN_X0_1 + 10) // 子窗口详细频谱 最左侧谱线的X轴开始点
#define FFT_SUBWIN_Y0 FFT_MAINWIN_Y0           // 子窗口详细频谱 Y轴开始点
//
#define FFT_SUBWIN_SPECTRUM_COLOR_1 CYAN_4_4              // 子窗口详细频谱的颜色
#define FFT_SUBWIN_SPECTRUM_COLOR_2 (RED_4_4 | GREEN_2_4) // 子窗口详细频谱的 2边及中间 频谱线的颜色
////////////////////////////////////////////////////////

extern uint16_t Last_Spectrum[FFT_MAINWIN_SPECT_TOTAL_DISP];

////////////////////////////////////////////////////////

void tftDisp_Test_Val(uint16_t, uint16_t); // 显示 valuetest 的值

void tftDisp_Touch_XY(struTouch *);
void Gui_Draw_A_Arrow(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor);
void Draw_MainWin_Block_Words(void);
void Draw_MainWin_Spectrum(float *fftfreq, uint32_t fftlength);
void Init_Last_Spectrum(uint16_t *lastvalue);
void Draw_xdB_Line(void);
void Draw_MainWin_Spectrum_Flag_Line(void);
void Draw_MainWin_Spectrum_Flag_Word(void);

void Inin_Win(void);

void DRAW_MainWin_Arrow_Freq(uint16_t);
void Write_SubWin_Left_Center_Freq(uint16_t);

void Draw_SubWin_FFT_Spectrum(uint16_t, float *);
void Draw_SubWin_Block(void);
////////////////////////////////////////////////////////
#endif
