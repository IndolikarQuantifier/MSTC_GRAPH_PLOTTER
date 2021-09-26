#pragma once

#include<Windows.h>
#include "../Error/Errors.h"

#define SUCCESS 0
#define FAILURE 1

#define LNE_START 0x01
#define LNE_END   0x02
#define LNE_COLOR 0x04

#define LNE_HSCROLL 0x08
#define LNE_VSCROLL 0x10

typedef int Status;
typedef int Options;

typedef struct tagLINE
{
    POINT StartPoint;
    POINT EndPoint;

    COLORREF LineColor;
}LINE,*PLINE,*LPLINE;


PLINE CreateLine();
Status UpdateLine(PLINE hLine,Options options,PLINE hInLine);
Status DeleteLine(PLINE hLine);

Status DrawLine(PLINE hLine,Options option);