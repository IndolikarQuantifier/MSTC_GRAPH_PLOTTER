#pragma once

#include<Windows.h>
#include<stdlib.h>
// #include "../Lines/Line.h"
#include "../CoOrdinateAxis/CoOrdinateAxis.h"
#include "../Error/Errors.h"
#include "../GenericDCLL/GDCLL.h"

#define LINE_GRAPH_SAVE 10
#define DEFAULT_GENESIS 96

#define LNE_VSCROLL 1
#define LNE_HSCROLL 2

typedef int status;
typedef unsigned long int scrollOption;

typedef struct tagBUFFERPOINTS
{
    POINT point;

    NODE link;
}BUFFERPOINTS,*LPBUFFERPOINTS,*PBUFFERPOINTS;

typedef struct tagLINE
{
    LPPOINT points;
    COLORREF color;

    unit* height;
    unit maxHeight;

    int nSize;

    LPPOINT lpDisplayBuffer;
    LPDCLL DisplayLineBuffer;

}LINE,*LPLINE,*PLINE;
typedef struct tagSIMPLELINEGRAPH
{
    int genesisPoint;
    int maxPoints;
    int pointGap;

    LPLINE Lines;
    LPCOORDINATEAXIS lpCoOrdinateAxis;
    LPSCALELINE lpScaleLine;

    int nSize;
}SIMPLELINEGRAPH,*LPSIMPLELINEGRAPH,*PSIMPLELINEGRAPH;

LPBUFFERPOINTS CreateBufferPoint(LONG x);
// HWND InitLineGraph(HINSTANCE hInstance,HWND hPrentWnd);
LRESULT CALLBACK WndSimpleLineGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int* ppPoints,int m,int scale,LPCOLORREF pColor);

status RelocateLines(LPSIMPLELINEGRAPH lpSimpleLineGraph);
status SetDisplayLineBuffer(LPSIMPLELINEGRAPH lpSimpleLineGraph,size_t size);
status DrawLines(HDC hdc,PAINTSTRUCT* ps,LPLINE lpLine,scrollOption option,int vPos,LPSCROLLINFO lpScrollInfo,COLORREF color,LONG axisY);
status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph);
status_t DestoryBufferPoint(LPNODE rm_node);