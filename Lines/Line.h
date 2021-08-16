#pragma once

#include<Windows.h>
#include "../Errors.h"

#define SUCCESS 1
#define FAILURE 0

#define LNE_POINTS 0x01
#define LNE_COLOR  0x02

typedef int status;
typedef int len_t;
typedef int option;

typedef struct tagLINE
{
    LPPOINT pPoints;
    COLORREF colLine;
    int nSize;
}LINE,*LPLINE,PLINE;

LPLINE CreateLine();
status SetLine(LPLINE lpLine,const LPLINE InLine,option options);
status DrawLine(HDC hdc,LPPAINTSTRUCT ps,LPLINE line,LPSCROLLINFO vScroll,LPSCROLLINFO hScroll);
status DestoryLine(LPLINE lpLine);