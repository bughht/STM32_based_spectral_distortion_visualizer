/*
 * @Author: Bughht 
 * @Date: 2021-05-11 12:30:57
 * @LastEditTime: 2021-05-24 16:23:46
 * @Description: Library for constructing the basement of Visualization Environment, including figure for plotting. 
 * Redefined the Line drawing functions for designing a much more user friendly drawing enviroment. Now then you could Draw a line between two points of float data and altering the thickness of the line was supported. 
 * TODO: Generate a Anti-aliasing line function. Something strange happened when I managed to add color with alpha value on to the original pixel.
 * @FilePath: \MDK-ARMe:\SHU\GCJY\BIG\hardware\windowsframe\windowsframe.c
 */
#include "windowsframe.h"
#include "adc.h"
#include "DataOperation.h"
#include "lcd_drive.h"

Figure Voltage_time_fig; //Figure of Voltage-time
Figure FFT_fig;          //Figure of FFT (Voltage-Frequency)

Screen_Button StopButton; //Button for stop the movement of figure

/**
 * @description: Get RGB values of a pixel
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} *red
 * @param {uint16_t} *green
 * @param {uint16_t} *blue
 * @return {*}
 */
void GetPoint_RGB(uint16_t x, uint16_t y, uint16_t *red, uint16_t *green, uint16_t *blue)
{
    WriteComm(0x2a); // 设置x坐标
    WriteData(x >> 8);
    WriteData(x & 0xff);

    WriteComm(0x2b); // 设置y坐标
    WriteData(y >> 8);
    WriteData(y & 0xff);

    WriteComm(0x2e);

    *red = *(__IO uint16_t *)(Bank1_LCD_D); // 保留，没有用到但是不能去除
    *red = *(__IO uint16_t *)(Bank1_LCD_D); // 保留，没有用到但是不能去除

    *green = (*(__IO uint16_t *)(Bank1_LCD_D)) & 0xff; // 绿颜色值 经LCD内部r矫正，输出为8位，也可以把低2位屏蔽掉, 即(*(__IO uint16_t *)(Bank1_LCD_D)) & 0xfc
    *red = (*(__IO uint16_t *)(Bank1_LCD_D)) & 0xff;   // 红颜色值 经LCD内部r矫正，输出为8位，也可以把低3位屏蔽掉, 即(*(__IO uint16_t *)(Bank1_LCD_D)) & 0xf8
    *blue = (*(__IO uint16_t *)(Bank1_LCD_D)) & 0xff;  // 蓝颜色值 经LCD内部r矫正，输出为8位，也可以把低3位屏蔽掉, 即(*(__IO uint16_t *)(Bank1_LCD_D)) & 0xf8
}

/**
 * @description: Collect RGB data of a pixel
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @return {*}
 */
uint16_t GetPoint_RGB565(uint16_t x, uint16_t y)
{
    uint16_t red, green, blue, tmp16;

    GetPoint_RGB(x, y, &red, &green, &blue);
    tmp16 = (((red & 0xf8) << 8) | ((green & 0xfc) << 3) | ((blue & 0xf8) >> 3));

    return tmp16;
}

/**
 * 
 * @description: Construct a Point
 * @param {float} X
 * @param {float} Y
 * @return {*}
 */
Pos_XY Point(float X, float Y)
{
    return (Pos_XY){X, Y};
}

COLOR crCalculateBlend(float a, float c1, float c2)
{
    return (COLOR)round(c1 * (1.0 - a) + c2 * a);
}

/**
 * @description: Blend Color with alpha
 * Warning: This function contains unreasonable bugs, please be careful when using it.
 * @param {Pos_XY} p0
 * @param {float} alpha
 * @param {COLOR} c
 * @return {*}
 */
void AlphaBlend(Pos_XY p, float alpha, COLOR c)
{
    if (alpha <= 0)
        return;
    COLOR pc = GetPoint_RGB565((uint64_t)p.X, (uint64_t)p.Y);
    COLOR pR = (pc >> 11);
    COLOR pG = ((pc & 0x07e0) >> 5);
    COLOR pB = (pc & 0x001f);
    COLOR lR = (c >> 11);
    COLOR lG = ((c & 0x07e0) >> 5);
    COLOR lB = (c & 0x001f);
    //COLOR R = ((COLOR)round(alpha * (float)(c >> 11) + (1 - alpha) * (float)pR));
    //COLOR G = ((COLOR)round(alpha * (float)((c & 0x07e0) >> 5) + (1 - alpha) * (float)pG));
    //COLOR B = ((COLOR)round(alpha * (float)(c & 0x001f) + (1 - alpha) * (float)pB));
    COLOR R = crCalculateBlend(alpha, (float)pR, (float)lR);
    COLOR G = crCalculateBlend(alpha, (float)pG, (float)lG);
    COLOR B = crCalculateBlend(alpha, (float)pB, (float)lB);
    R = R > 0x001f ? 0x001f : R;
    G = G > 0x003f ? 0x003f : G;
    B = B > 0x001f ? 0x001f : B;
    R = R < 0 ? 0 : R;
    G = G < 0 ? 0 : G;
    B = B < 0 ? 0 : B;

    COLOR ca = (R << 11) + (G << 5) + B;
    Gui_Draw_aPixel(p.X, p.Y, ca);
}

/**
 * @description: Capsule signed distance field Alogrithm
 * @param {Pos_XY} p0
 * @param {Pos_XY} p1
 * @param {Pos_XY} p2
 * @param {float} r
 * @return {*}
 */
float capsuleSDF(Pos_XY p, Pos_XY a, Pos_XY b, float r)
{
    float pax = p.X - a.X;
    float pay = p.Y - a.Y;
    float bax = b.X - a.X;
    float bay = b.Y - a.Y;
    float h = max(min((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    //return dx * dx + dy * dy < r * r;
    return sqrtf(dx * dx + dy * dy) - r;
}

/**
 * @description: Basic Capsule Alogrithm
 * @param {Pos_XY} p0
 * @param {Pos_XY} p1
 * @param {Pos_XY} p2
 * @param {float} r
 * @return {*}
 */
float capsule(Pos_XY p, Pos_XY a, Pos_XY b, float r)
{
    float pax = p.X - a.X;
    float pay = p.Y - a.Y;
    float bax = b.X - a.X;
    float bay = b.Y - a.Y;
    float h = max(min((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    return dx * dx + dy * dy < r * r;
    //return sqrtf(dx * dx + dy * dy) - r;
}

/**
 * @description: get max value within the list
 * @param {float} *list
 * @param {int} n
 * @return {*}
 */
float list_max(float *list, int n)
{
    int i;
    float out = list[0];
    for (i = 0; i < n; i++)
        if (out < list[i])
            out = list[i];
    return out;
}

/**
 * @description: get min value within the list
 * @param {float} *list
 * @param {int} n
 * @return {*}
 */
float list_min(float *list, int n)
{
    int i;
    float out = list[0];
    for (i = 0; i < n; i++)
        if (out > list[i])
            out = list[i];
    return out;
}

/**
 * @description: Drawing Line Using AABB Optimization with capsule, thickness available
 * @param {Pos_XY} p0: Point Start 
 * @param {Pos_XY} p1: Point End
 * @param {float} r: Line Thickness
 * @param {COLOR} c: Color
 * @return {*}
 */
void Windows_Draw_Line(Pos_XY p0, Pos_XY p1, float r, COLOR c)
{
    int x0 = (int)floorf(min(p0.X, p1.X) - r);
    int x1 = (int)ceilf(max(p0.X, p1.X) + r);
    int y0 = (int)floorf(min(p0.Y, p1.Y) - r);
    int y1 = (int)ceilf(max(p0.Y, p1.Y) + r);

    for (int y = y0; y <= y1; y++)
        for (int x = x0; x <= x1; x++)
            if (capsule(Point(x, y), p0, p1, r))
                Gui_Draw_aPixel(x, y, c);
    //AlphaBlend(Point(x, y), max(min((0.5F - capsuleSDF(Point(x, y), p0, p1, r)), 1.0F), 0.0F), c);
}

/**
 * @description: Better Line Using SDF Algorithm with AABB Optimization
 * @param {Pos_XY} p0: Point Start 
 * @param {Pos_XY} p1: Point End
 * @param {float} r: Line Thickness
 * @param {COLOR} c: Color
 * @return {*}
 */
void Windows_Draw_Line_Smooth(Pos_XY p0, Pos_XY p1, float r, COLOR c)
{
    int x0 = (int)floorf(min(p0.X, p1.X) - r);
    int x1 = (int)ceilf(max(p0.X, p1.X) + r);
    int y0 = (int)floorf(min(p0.Y, p1.Y) - r);
    int y1 = (int)ceilf(max(p0.Y, p1.Y) + r);

    for (int y = y0; y <= y1; y++)
        for (int x = x0; x <= x1; x++)
            if (capsuleSDF(Point(x, y), p0, p1, r))
                //Gui_Draw_aPixel(x, y, c);
                AlphaBlend(Point(x, y), max(min((0.5F - capsuleSDF(Point(x, y), p0, p1, r)), 1.0F), 0.0F), c);
}

/**
 * @description: Draw the Rectangle on the screen.
 * @param {Rect} R: Rect Parameters
 * @param {float} r: Line Thickness
 * @param {COLOR} c: Color
 * @return {*}
 */
void Windows_Draw_Rectangle(Rect R, float r, COLOR c)
{
    Pos_XY P1 = R.P1;
    Pos_XY P2 = Point(R.P1.X, R.P2.Y);
    Pos_XY P3 = R.P2;
    Pos_XY P4 = Point(R.P2.X, R.P1.Y);
    Windows_Draw_Line(P1, P2, r, c);
    Windows_Draw_Line(P2, P3, r, c);
    Windows_Draw_Line(P3, P4, r, c);
    Windows_Draw_Line(P4, P1, r, c);
}

/**
 * @description: Plot a Polyline on the screen
 * @param {Pos_XY} *p
 * @param {int} n
 * @param {float} r
 * @param {COLOR} c
 * @return {*}
 */
void Windows_Draw_Polyline(Pos_XY *p, int n, float r, COLOR c)
{
    for (int i = 0; i < n - 1; i++)
    {
        Windows_Draw_Line(p[i], p[i + 1], r, c);
    }
}

/**
 * @description: Construction the rectangle
 * @param {Pos_XY} p0
 * @param {Pos_XY} p1
 * @return {*}
 */
Rect Rectangle(Pos_XY p0, Pos_XY p1)
{
    return (Rect){p0, p1};
}

/**
 * @description:  Initialize the position and plot type of the figure
 * @param {Pos_XY} p0
 * @param {Pos_XY} p1
 * @param {Plot_TYPE} type
 * @return {*}
 */
Figure Figure_init(Pos_XY p0, Pos_XY p1)
{
    Figure fig;
    fig.FigSize = Rectangle(p0, p1);
    fig.Fig_X = p1.X - p0.X;
    fig.Fig_Y = p1.Y - p0.Y;
    fig.xlegendnum = 10;
    fig.ylegendnum = 5;
    fig.backcolor = BLACK;
    fig.Legend = TRUE;
    return fig;
}

/**
 * @description: Bound data to the fig
 * @param {Figure} *fig
 * @param {void} *data
 * @return {*}
 */
Figure *Figure_bound_data(Figure *fig, float *init_data, int init_datalen)
{
    fig->data = init_data;
    fig->datalen = init_datalen;
    fig->yrange[0] = list_min(init_data, init_datalen);
    fig->yrange[1] = list_max(init_data, init_datalen);
    fig->ylim = (fig->yrange[1] - fig->yrange[0]) / 5;
    return fig;
}

/**
 * @description: Plot the structure of figure on the screen (without data)
 * @param {Figure} *fig
 * @return {*}
 */
void Figure_window_plot(Figure *fig)
{
    Lcd_Clear_Some_Area(fig->FigSize.P1.X, fig->FigSize.P1.Y, fig->FigSize.P2.X, fig->FigSize.P2.Y, fig->backcolor);
    Windows_Draw_Rectangle(fig->FigSize, 4, WHITE_4_4);
}

/**
 * @description: Plot Line of the data of fig after updating the data 
 * @param {Figure} *fig
 * @param {float} linewidth
 * @param {COLOR} c
 * @return {*}
 */
void Figure_line_plot(Figure *fig, float linewidth, COLOR c)
{

    Lcd_Clear_Some_Area(fig->FigSize.P1.X, fig->FigSize.P1.Y, fig->FigSize.P2.X, fig->FigSize.P2.Y, fig->backcolor); //Flush the lcd Area

    int len = fig->datalen;
    fig->yrange[0] = list_min(fig->data, len);
    fig->yrange[1] = list_max(fig->data, len);
    Pos_XY plist[SIZEofADC1];
    int i;
    char buff[50];
    float data_ymin = fig->yrange[0] - (fig->yrange[1] - fig->yrange[0]) / (fig->ylegendnum - 3);
    float data_ymax = fig->yrange[1] + (fig->yrange[1] - fig->yrange[0]) / (fig->ylegendnum - 3);
    float windows_ymin = fig->FigSize.P2.Y - 3;
    float windows_ymax = fig->FigSize.P1.Y + 3;
    float data_ypp = (data_ymax - data_ymin) <= 0 ? 0.000001f : (data_ymax - data_ymin);
    float y_ratio = (windows_ymin - windows_ymax) / data_ypp; // (data_ymax - data_ymin);

    float windows_xmin = fig->FigSize.P1.X + 3;
    float windows_xmax = fig->FigSize.P2.X - 3;
    float windows_xlim = (windows_xmax - windows_xmin) / len;
    float x_ratio = (windows_xmax - windows_xmin) / (fig->xrange[1] - fig->xrange[0]);
    if (fig->Legend == TRUE)
    {
        fig->xlim = (fig->xrange[1] - fig->xrange[0]) / (fig->xlegendnum - 1);
        fig->ylim = (data_ymax - data_ymin) / (fig->ylegendnum - 1);
        for (i = 0; i < fig->xlegendnum; i++)
        {
            Windows_Draw_Line(Point(windows_xmin + i * x_ratio * fig->xlim, windows_ymax), Point(windows_xmin + i * x_ratio * fig->xlim, windows_ymin), 2, WHITE_1_4); //Grid Gray Vertical lines of the mesh
            sprintf(buff, "     ");
            Gui_Draw_Asc16_Hzk16(windows_xmin + i * x_ratio * fig->xlim - 13, windows_ymin + 10, WHITE_4_4, BLACK, buff);
            sprintf(buff, "%.1f", fig->xrange[0] + i * fig->xlim);
            Gui_Draw_Asc16_Hzk16(windows_xmin + i * x_ratio * fig->xlim - 13, windows_ymin + 10, WHITE_4_4, BLACK, buff); //Print x axis numbers
        }
        Gui_Draw_Asc16_Hzk16(fig->FigSize.P2.X - 30, windows_ymin + 26, WHITE_4_4, BLACK, fig->xlabel); //Print x label
        for (i = 0; i < fig->ylegendnum; i++)
        {
            Windows_Draw_Line(Point(windows_xmax, windows_ymin - i * y_ratio * fig->ylim), Point(windows_xmin, windows_ymin - i * y_ratio * fig->ylim), 2, WHITE_1_4); //Grid Gray horizontal lines of the mesh
            sprintf(buff, "       ");
            Gui_Draw_Asc16_Hzk16(windows_xmin - 64, windows_ymin - i * y_ratio * fig->ylim - 5, WHITE_4_4, BLACK, buff);
            sprintf(buff, "%.3f", data_ymin + i * fig->ylim);
            Gui_Draw_Asc16_Hzk16(windows_xmin - 55, windows_ymin - i * y_ratio * fig->ylim - 5, WHITE_4_4, BLACK, buff); //Print y axis numbers
        }
        Gui_Draw_Asc16_Hzk16(windows_xmin - 55, windows_ymax - 25, WHITE_4_4, BLACK, fig->ylabel); //Print y label
    }

    /*Plot the line of data*/
    for (i = 0; i < len; i++)
    {
        plist[i].X = windows_xmin + windows_xlim * i;
        plist[i].Y = windows_ymin - y_ratio * (fig->data[i] - data_ymin);
        if (i != 0)
            Windows_Draw_Line(plist[i - 1], plist[i], linewidth, c);
    }
    Gui_Draw_Asc16_Hzk16((windows_xmin + windows_xmax) / 2 - 8.0 * strlen(fig->title) / 2, windows_ymax - 25, WHITE_4_4, BLACK, fig->title); //Print the title above the figure
}

/**
 * @description: Initialize the button structure
 * @param {Pos_XY} p0
 * @param {Pos_XY} p1
 * @param {char} *uplabel
 * @param {char} *downlabel
 * @param {COLOR} upcolor
 * @param {COLOR} downcolor
 * @param {COLOR} labelcolor
 * @return {*}
 */
Screen_Button Button_init(Pos_XY p0, Pos_XY p1, char *uplabel, char *downlabel, COLOR upcolor, COLOR downcolor, COLOR labelcolor)
{
    Screen_Button button;
    button.ButtonSize = Rectangle(p0, p1);
    button.UPcolor = upcolor;
    button.DOWNcolor = downcolor;
    strcpy(button.UPlabel, uplabel);
    strcpy(button.DOWNlabel, downlabel);
    button.ButtonDOWN = FALSE;
    button.labelcolor = labelcolor;
    return button;
}

/**
 * @description: Plot the button (Auto examing whether the button is pressed)
 * @param {Screen_Button} *button
 * @return {*}
 */
void Button_plot(Screen_Button *button)
{
    int lenup, lendown;
    lenup = strlen(button->UPlabel) - 1;
    lendown = strlen(button->DOWNlabel) - 1;
    int xmid = (int)(button->ButtonSize.P2.X + button->ButtonSize.P1.X) / 2;
    int ymid = (int)(button->ButtonSize.P2.Y + button->ButtonSize.P1.Y) / 2;

    if (button->ButtonDOWN == FALSE)
    {
        Lcd_Clear_Some_Area(button->ButtonSize.P1.X, button->ButtonSize.P1.Y, button->ButtonSize.P2.X, button->ButtonSize.P2.Y, button->UPcolor);
        Gui_Draw_Asc24_Hzk24(xmid - lenup * 6 - 12, ymid - 12, button->labelcolor, button->UPcolor, button->UPlabel);
    }
    else
    {
        Lcd_Clear_Some_Area(button->ButtonSize.P1.X, button->ButtonSize.P1.Y, button->ButtonSize.P2.X, button->ButtonSize.P2.Y, button->DOWNcolor);
        Gui_Draw_Asc24_Hzk24(xmid - lendown * 6 - 12, ymid - 12, button->labelcolor, button->DOWNcolor, button->DOWNlabel);
    }
}

/**
 * @description: Check whether the position of touch point is within the size of the button
 * @param {Screen_Button} *button
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @return {*}
 */
BOOL Button_Checkpos(Screen_Button *button, uint16_t x, uint16_t y)
{
    if (x > button->ButtonSize.P1.X && y > button->ButtonSize.P1.Y && x < button->ButtonSize.P2.X && y < button->ButtonSize.P2.Y)
        return TRUE;
    else
        return FALSE;
}

/**
 * @description: Check wheter the position of touch point is within the size of the figure
 * @param {Figure} *fig
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @return {*}
 */
BOOL Figure_Checkpos(Figure *fig, uint16_t x, uint16_t y)
{
    if (x > fig->FigSize.P1.X && y > fig->FigSize.P1.Y && x < fig->FigSize.P2.X && y < fig->FigSize.P2.Y)
        return TRUE;
    else
        return FALSE;
}