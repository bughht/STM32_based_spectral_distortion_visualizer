/*************************************************
// 文件说明:      此文件为基本的图形显示驱动函数集
//                建立在在底层驱动文件之上，用户可以增加相应的图形功能
//                这里基本的函数是画点函数，其他的建立在此函数之上，软件可以移植
// 创 建 人：     ZHAOWL
// 日期：         2017-5-10
// 修 改 人：                     
// 修改日期：     201x-xx-xx
*************************************************/
#include "gui.h"

/*************************************************
// 函 数 名:      Gui_Draw_aPixel(uint16_t x,uint16_t y,uint16_t color)
// 函数功能描述:  画一个点. 此函数为以下函数的基础, 也可以归为硬件接口层的函数
// 参数描述:      u16 x,u16 y: 坐标; u16 color: 颜色数据
// 返 回 值:      无       
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件功能层

// 创 建 人：     ZHAOWL
// 日期：         2017-5-10
// 修 改 人：                     
// 修改日期：     201x-xx-xx
*************************************************/
void Gui_Draw_aPixel(uint16_t x, uint16_t y, uint16_t color)
{
    Lcd_Set_XY(x, y);
    *(__IO uint16_t *)(Bank1_LCD_D) = color;
}

/****************************************************************************
// 函 数 名:      Gui_Circle(u16 X0,u16 Y0,u16 R,u16 fc)
// 函数功能描述:  Bresenham算法 画圆
// 参数描述:      u16 X0,u16 Y0:颜色坐标; u16 R:半径; u16 fc:颜色值
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Circle(uint16_t X0, uint16_t Y0, uint16_t R, uint16_t fc)
{
    int x, y, d;

    x = 0;
    y = R;
    d = 3 - 2 * R; // 决策初始值
    while (x < y)
    {
        Gui_Draw_aPixel(X0 + x, Y0 + y, fc); //        7 对称画出8个点
        Gui_Draw_aPixel(X0 - x, Y0 + y, fc); //        6
        Gui_Draw_aPixel(X0 + x, Y0 - y, fc); //        2
        Gui_Draw_aPixel(X0 - x, Y0 - y, fc); //        3
        Gui_Draw_aPixel(X0 + y, Y0 + x, fc); //        8
        Gui_Draw_aPixel(X0 - y, Y0 + x, fc); //        5
        Gui_Draw_aPixel(X0 + y, Y0 - x, fc); //        1
        Gui_Draw_aPixel(X0 - y, Y0 - x, fc); //        4

        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    if (x == y)
    {
        Gui_Draw_aPixel(X0 + x, Y0 + y, fc); //  对称画出8个点
        Gui_Draw_aPixel(X0 - x, Y0 + y, fc);
        Gui_Draw_aPixel(X0 + x, Y0 - y, fc);
        Gui_Draw_aPixel(X0 - x, Y0 - y, fc);
        Gui_Draw_aPixel(X0 + y, Y0 + x, fc);
        Gui_Draw_aPixel(X0 - y, Y0 + x, fc);
        Gui_Draw_aPixel(X0 + y, Y0 - x, fc);
        Gui_Draw_aPixel(X0 - y, Y0 - x, fc);
    }
}

/****************************************************************************
// 函 数 名:      Gui_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color) 
// 函数功能描述:  画线函数，使用Bresenham 画直线算法
//0=<k<1, p0=2*dy - dx; 当 pk<0 时 下一个点坐标为 (xk + 1,yk), pk+1 = pk+2*dy; 当 pk>=0 时下一个点坐标为 (xk + 1,yk + 1), pk+1= pk + 2*dy - 2×dx; 
//-1<k<0, p0=2*dy + dx; 当 pk>0 时 下一个点坐标为 (xk + 1,yk), pk+1 = pk+2*dy; 当 pk<=0 时下一个点坐标为 (xk + 1,yk - 1), pk+1= pk + 2*dy + 2×dx; 
// 参数描述:      u16 x0, u16 y0, u16 x1, u16 y1:起点终点坐标; u16 Color:颜色值
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Color)
{
    int dx, dy, dx2, dy2, x_inc, y_inc, error, index, xx0, yy0;

    xx0 = x0;
    yy0 = y0;

    Lcd_Set_XY(x0, y0);
    dx = x1 - x0; //计算x距离
    dy = y1 - y0; //计算y距离

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) //x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点 //且线的点数等于x距离，以x轴递增画点
    {
        error = dy2 - dx;
        for (index = 0; index <= dx; index++) //要画的点数不会超过x距离
        {
            Gui_Draw_aPixel(xx0, yy0, Color); //画点
            if (error >= 0)                   //是否需要增加y坐标值
            {
                error -= dx2;
                yy0 += y_inc; //增加y坐标值
            }
            error += dy2; // adjust the error term
            xx0 += x_inc; //x坐标值每次画点后都递增1 // move to the next pixel
        }
    }
    else //y轴大于x轴，则每个y轴上只有一个点，x轴若干个点  //以y轴为递增画点
    {
        error = dx2 - dy;                     // initialize error term
        for (index = 0; index <= dy; index++) // draw the line
        {
            Gui_Draw_aPixel(xx0, yy0, Color); // set the pixel
            if (error >= 0)                   // test if error overflowed
            {
                error -= dy2;
                xx0 += x_inc; // move to next line
            }
            error += dx2;
            yy0 += y_inc;
        }
    }
}

/****************************************************************************
// 函 数 名:      Gui_Draw_Rect(u16 x0, u16 y0, u16 x1, u16 y1, u16 linecolor) 
// 函数功能描述:  画矩形
// 参数描述:      (x0,y0) 左上角点的坐标,  (x1,y1) 右下角点的坐标; u16 linecolor: 边界颜色
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_Rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t linecolor)
{
    Gui_Draw_Line(x0, y0, x1, y0, linecolor);
    Gui_Draw_Line(x0, y0, x0, y1, linecolor);
    Gui_Draw_Line(x0, y1, x1, y1, linecolor);
    Gui_Draw_Line(x1, y0, x1, y1, linecolor);
}

/****************************************************************************
// 函 数 名:      Gui_Draw_Asc16_Hzk16(u16 x, u16 y, u16 forcolor, u16 bkcolor, u8 *string) 
// 函数功能描述:  写16×16点的字符串
// 参数描述:      u16 x, u16 y:左上角点的起始坐标; u16 forcolor:前景色; u16 bkcolor: 背景色; u8 *string:字符串 
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_Asc16_Hzk16(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t *string)
{
    uint32_t i, j, ch1;
    uint8_t str[HZK16_nByte + 1], tmp;
    uint16_t x0, y0;

    x0 = x;
    y0 = y;

    while (*string)
    {
        if ((*string) < 0x80)
        {
            ch1 = (uint32_t)(*string++);
            Read_aAsc16_Dot(ch1, str);
            Lcd_Set_Region(x0, y0, (x0 + 7), (y0 + ASC16_nLINE - 1));
            for (i = 0; i < ASC16_nLINE; i++)
            {
                tmp = str[i];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
            }
            x0 += 8;
        }
        else
        {
            ch1 = (uint32_t)(*string++);
            ch1 <<= 8;
            ch1 |= *string++;
            Read_aWord_Hzk16_Dot(ch1, str);
            Lcd_Set_Region(x0, y0, (x0 + 15), (y0 + ASC16_nLINE - 1));
            for (i = 0; i < HZK16_nLINE; i++)
            {
                tmp = str[2 * i];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
                tmp = str[2 * i + 1];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
            }
            x0 += 16;
        }
    }
    Lcd_Set_Region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
}

/****************************************************************************
// 函 数 名:      Gui_Draw_A_Asc16(u16 x, u16 y, u16 forcolor, u16 bkcolor, u8 chr) 
// 函数功能描述:  写8×16点的一个ASCII字符
// 参数描述:      u16 x, u16 y:左上角点的起始坐标; u16 forcolor:前景色; u16 bkcolor: 背景色; u8 chr:字符 
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_A_Asc16(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t chr)
{
    uint32_t i, j;
    uint8_t str[HZK16_nByte + 1], tmp;

    Read_aAsc16_Dot(chr, str);
    Lcd_Set_Region(x, y, (x + 7), (y + ASC16_nLINE - 1));
    for (i = 0; i < ASC16_nLINE; i++)
    {
        tmp = str[i];
        for (j = 0; j < 8; j++)
        {
            if (tmp & 0x80)
            {
                *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
            }
            else
            {
                *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
            }
            tmp <<= 1;
        }
    }
    Lcd_Set_Region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
}

/****************************************************************************
// 函 数 名:      Gui_Draw_Asc24_Hzk24(u16 x, u16 y, u16 forcolor, u16 bkcolor, u8 *string) 
// 函数功能描述:  写24×24点的中英文字符串
// 参数描述:      u16 x, u16 y:左上角点的起始坐标; u16 forcolor:前景色; u16 bkcolor: 背景色; u8 *string:字符串 
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_Asc24_Hzk24(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t *string)
{
    uint32_t i, j, ch1;
    uint8_t str[HZK24_nByte + 1], tmp;
    uint16_t x0, y0;

    x0 = x;
    y0 = y;

    while (*string)
    {
        if ((*string) < 0x80)
        {
            ch1 = (uint32_t)(*string);
            string++;
            Read_aAsc24_Dot(ch1, str);
            Lcd_Set_Region(x0, y0, (x0 + 11), (y0 + ASC24_nLINE - 1));
            for (i = 0; i < ASC24_nLINE; i++)
            {
                tmp = str[2 * i];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
                tmp = str[2 * i + 1];
                for (j = 0; j < 4; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
            }
            x0 += 12;
        }
        else
        {
            ch1 = (uint32_t)(*string);
            string++;
            ch1 <<= 8;
            ch1 |= *string;
            string++;
            Read_aWord_Hzk24_Dot(ch1, str);
            Lcd_Set_Region(x0, y0, (x0 + 23), (y0 + HZK24_nLINE - 1));
            for (i = 0; i < HZK24_nLINE; i++)
            {
                tmp = str[3 * i];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }

                tmp = str[3 * i + 1];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
                tmp = str[3 * i + 2];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
            }
            x0 += 24;
        }
    }
    Lcd_Set_Region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
}

/****************************************************************************
// 函 数 名:      Gui_Draw_Asc32_String(u16 x, u16 y, u16 forcolor, u16 bkcolor, u8 *string) 
// 函数功能描述:  写32×32点的英文字符串
// 参数描述:      u16 x, u16 y:左上角点的起始坐标; u16 forcolor:前景色; u16 bkcolor: 背景色; u8 *string:字符串 
// 返 回 值:      无    
// 返回值描述:    无
// 全局变量:      无
// 函数状态：     已经完成测试
// 函数层次：     软件应用层

// 创 建 人：     zhaoweiliang
// 日期：         2016-3-20
// 修 改 人：                     
// 修改日期：     201x-xx-xx
****************************************************************************/
void Gui_Draw_Asc32_String(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t *string)
{
    uint32_t i, j, ch1;
    uint8_t str[ASC32_nByte + 1], tmp;
    uint16_t x0, y0;

    x0 = x;
    y0 = y;

    while (*string)
    {
        if ((*string) < 0x80)
        {
            ch1 = (uint32_t)(*string);
            string++;
            Read_aAsc32_Dot(ch1, str);
            Lcd_Set_Region(x0, y0, (x0 + 15), (y0 + ASC32_nLINE - 1));
            for (i = 0; i < ASC32_nLINE; i++)
            {
                tmp = str[2 * i];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
                tmp = str[2 * i + 1];
                for (j = 0; j < 8; j++)
                {
                    if (tmp & 0x80)
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = forcolor;
                    }
                    else
                    {
                        *(__IO uint16_t *)(Bank1_LCD_D) = bkcolor;
                    }
                    tmp <<= 1;
                }
            }
            x0 += 16;
        }
    }
    Lcd_Set_Region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示一个无符号指定长度的十进制数
void GUI_A_usigned_Decimal(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint32_t digital, uint8_t numericprecision, uniWORDDOTSTYPE dotstype)
{ //     X坐标        Y坐标        前景颜色           背景颜色         显示的数          显示的位数               显示数字的点数
    char dig[30];
    memset(dig, '\0', 30);

    switch (numericprecision)
    {
    case 1:
    {
        sprintf(dig, "%1d", digital);
        break;
    }
    case 2:
    {
        sprintf(dig, "%2d", digital);
        break;
    }
    case 3:
    {
        sprintf(dig, "%3d", digital);
        break;
    }
    case 4:
    {
        sprintf(dig, "%4d", digital);
        break;
    }
    case 5:
    {
        sprintf(dig, "%5d", digital);
        break;
    }
    case 6:
    {
        sprintf(dig, "%6d", digital);
        break;
    }
    case 7:
    {
        sprintf(dig, "%7d", digital);
        break;
    }
    case 8:
    {
        sprintf(dig, "%8d", digital);
        break;
    }
    case 9:
    {
        sprintf(dig, "%9d", digital);
        break;
    }
    default:
    {
        sprintf(dig, "%d", digital);
        break;
    } // 缺省
    }

    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    }
    }
}

void GUI_A_signed_Decimal(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, int digital, uint8_t numericprecision, uniWORDDOTSTYPE dotstype)
{ //     X坐标        Y坐标        前景颜色           背景颜色        显示的数          显示的位数           显示数字的点数
    char dig[30];
    memset(dig, '\0', 30);

    switch (numericprecision)
    {
    case 1:
    {
        sprintf(dig, "%1d", digital);
        break;
    }
    case 2:
    {
        sprintf(dig, "%2d", digital);
        break;
    }
    case 3:
    {
        sprintf(dig, "%3d", digital);
        break;
    }
    case 4:
    {
        sprintf(dig, "%4d", digital);
        break;
    }
    case 5:
    {
        sprintf(dig, "%5d", digital);
        break;
    }
    case 6:
    {
        sprintf(dig, "%6d", digital);
        break;
    }
    case 7:
    {
        sprintf(dig, "%7d", digital);
        break;
    }
    case 8:
    {
        sprintf(dig, "%8d", digital);
        break;
    }
    case 9:
    {
        sprintf(dig, "%9d", digital);
        break;
    }
    default:
    {
        sprintf(dig, "%d", digital);
        break;
    }
    }

    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    }
    }
}

// 显示一个无符号指定长度的十六进制数
void GUI_A_Uint8_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint8_t digital, uniWORDDOTSTYPE dotstype)
{ //   X坐标        Y坐标        前景颜色           背景颜色         显示的数          显示数字的点数
    char dig[20], buf[20];
    memset(dig, '\0', 20);
    memset(buf, '\0', 20);
    strcpy(buf, "0x");
    sprintf(dig, "%x", digital);
    strcat(buf, dig);
    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    }
    }
}

// 显示一个无符号指定长度的十六进制数
void GUI_A_Uint16_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint16_t digital, uniWORDDOTSTYPE dotstype)
{ //   X坐标        Y坐标        前景颜色           背景颜色         显示的数          显示数字的点数
    char dig[20], buf[20];
    memset(dig, '\0', 20);
    memset(buf, '\0', 20);
    strcpy(buf, "0x");
    sprintf(dig, "%x", digital);
    strcat(buf, dig);
    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    }
    }
}

// 显示一个无符号指定长度的十六进制数
void GUI_A_Uint32_Hex(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, uint32_t digital, uniWORDDOTSTYPE dotstype)
{ //   X坐标        Y坐标        前景颜色           背景颜色         显示的数          显示数字的点数
    char dig[20], buf[20];
    memset(dig, '\0', 20);
    memset(buf, '\0', 20);
    strcpy(buf, "0x");
    sprintf(dig, "%x", digital);
    strcat(buf, dig);
    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)buf);
        break;
    }
    }
}
/////////////////////////////////////////////////////////////////////////////
// 显示float 型的一个数
void GUI_aFloat(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, float digital, uint8_t numericprecision, uniWORDDOTSTYPE dotstype)
{ //   X坐标        Y坐标        前景颜色           背景颜色         显示的数        小数部分的位数             显示数字的点数
    char dig[30];
    memset(dig, '\0', 30);

    switch (numericprecision)
    {
    case 0:
    {
        sprintf(dig, "%.0f", digital);
        break;
    } // 不显示小数部分，四舍五入
    case 1:
    {
        sprintf(dig, "%.1f", digital);
        break;
    } // 显示1位小数，四舍五入
    case 2:
    {
        sprintf(dig, "%.2f", digital);
        break;
    } // 显示2位小数，四舍五入
    case 3:
    {
        sprintf(dig, "%.3f", digital);
        break;
    } // 显示3位小数，四舍五入
    case 4:
    {
        sprintf(dig, "%.4f", digital);
        break;
    } // 显示4位小数，四舍五入
    case 5:
    {
        sprintf(dig, "%.5f", digital);
        break;
    } // 显示5位小数，四舍五入
    default:
    {
        sprintf(dig, "%f", digital);
        break;
    } // 显示6位小数，四舍五入，float型小数部分精度6位
    }

    switch (dotstype)
    {
    case DOT16X16aWORD:
    {
        Gui_Draw_Asc16_Hzk16(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 16*8 点阵模式显示数值
    case DOT24X24aWORD:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 24*12点阵模式显示数值
    case DOT32X16aCHAR:
    {
        Gui_Draw_Asc32_String(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    } // 32*16点阵模式显示数值
    default:
    {
        Gui_Draw_Asc24_Hzk24(x, y, forcolor, bkcolor, (uint8_t *)dig);
        break;
    }
    }
}
