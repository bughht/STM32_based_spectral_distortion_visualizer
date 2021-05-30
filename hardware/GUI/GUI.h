#ifndef __GUI_H__
#define __GUI_H__

#include "Lcd_Drive.h"
#include "w25q64.h"

typedef enum
{
    DOT16X16aWORD = 1,
    DOT24X24aWORD,
    DOT32X16aCHAR,
} uniWORDDOTSTYPE;

typedef enum
{
    DIG_UINT8_T = 8,
    DIG_UINT16_T = 16,
    DIG_UINT32_T = 32,
    DIG_UINT64_T = 64,
} uniDIGITTYPE;

//////////////////////////////////////////////////////////////////
void Gui_Draw_aPixel(uint16_t, uint16_t, uint16_t);
void Gui_Draw_Line(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void Gui_Circle(uint16_t, uint16_t, uint16_t, uint16_t);
void Gui_Draw_Rect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void Gui_Draw_Asc16_Hzk16(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *);
void Gui_Draw_Asc24_Hzk24(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *);
void Gui_Draw_A_Asc16(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void Gui_Draw_Asc32_String(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *);

void GUI_A_usigned_Decimal(uint16_t, uint16_t, uint16_t, uint16_t, uint32_t, uint8_t, uniWORDDOTSTYPE);
void GUI_A_signed_Decimal(uint16_t, uint16_t, uint16_t, uint16_t, int, uint8_t, uniWORDDOTSTYPE);
void GUI_aFloat(uint16_t, uint16_t, uint16_t, uint16_t, float, uint8_t, uniWORDDOTSTYPE); // 显示float 型的一个数
void GUI_A_Uint8_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t digital, uniWORDDOTSTYPE dotstype);
void GUI_A_Uint16_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint16_t digital, uniWORDDOTSTYPE dotstype);
void GUI_A_Uint32_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint32_t digital, uniWORDDOTSTYPE dotstype);
#endif
