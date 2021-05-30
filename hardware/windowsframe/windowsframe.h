/*
 * @Author: Bughht
 * @Date: 2021-05-11 12:29:47
 * @LastEditTime: 2021-05-22 12:30:29
 * @Description: Library for constructing the basement of Visualization Environment, including figure for plotting. 
 * Redefined the Line drawing functions for designing a much more user friendly drawing enviroment. Now then you could Draw a line between two points of float data and altering the thickness of the line was supported. 
 * TODO: Generate a Anti-aliasing line function. Something strange happened when I managed to add color with alpha value on to the original pixel.
 * @FilePath: \MDK-ARMe:\SHU\GCJY\BIG\hardware\windowsframe\windowsframe.h
 */
#ifndef __WINDOWSFRAME_H__
#define __WINDOWSFRAME_H__

#include "common.h"
#include "GUI.h"
#include "lcd_drive.h"
#include "touch.h"
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef uint16_t COLOR;

typedef struct
{
    float X;
    float Y;
} Pos_XY;

typedef struct
{
    Pos_XY P1;
    Pos_XY P2;
} Rect;

typedef struct
{
    Rect FigSize;
    float Fig_X;
    float Fig_Y;
    float *data;
    int datalen;
    float xrange[2];
    float yrange[2];
    float xlim;
    float ylim;
    int xlegendnum;
    int ylegendnum;
    char xlabel[20];
    char ylabel[20];
    BOOL Legend;
    COLOR backcolor;
    char title[20];
} Figure;

typedef struct
{
    Rect ButtonSize;
    char UPlabel[20];
    char DOWNlabel[20];
    COLOR UPcolor;
    COLOR DOWNcolor;
    COLOR labelcolor;
    BOOL ButtonDOWN;
} Screen_Button;

extern Figure Voltage_time_fig;
extern Figure FFT_fig;

extern Screen_Button StopButton;

Pos_XY Point(float X, float Y);
Rect Rectangle(Pos_XY p0, Pos_XY p1);

float list_max(float *list, int n);
float list_min(float *list, int n);

void AlphaBlend(Pos_XY p0, float alpha, COLOR c);
void Windows_Draw_Line(Pos_XY p0, Pos_XY p1, float r, COLOR c);
void Windows_Draw_Rectangle(Rect R, float r, COLOR c);
void Windows_Draw_Polyline(Pos_XY *p, int n, float r, COLOR c);
float capsuleSDF(Pos_XY p0, Pos_XY p1, Pos_XY p2, float r);

Figure Figure_init(Pos_XY p0, Pos_XY p1);
Figure *Figure_bound_data(Figure *fig, float *data, int datalen);
void Figure_window_plot(Figure *fig);
void Figure_line_plot(Figure *fig, float linewidth, COLOR c);
void Figure_touch();
BOOL Figure_Checkpos(Figure *fig, uint16_t x, uint16_t y);

Screen_Button Button_init(Pos_XY p0, Pos_XY p1, char *uplabel, char *downlabel, COLOR upcolor, COLOR downcolor, COLOR labelcolor);
void Button_plot(Screen_Button *button);
BOOL Button_Checkpos(Screen_Button *button, uint16_t x, uint16_t y);
//void Proc_Button_pressed(Screen_Button *button, struTouch *cptxy);

#endif
