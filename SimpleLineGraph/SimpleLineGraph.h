#pragma once

#include<Windows.h>
#include<stdlib.h>
#include "../Lines/Line.h"
#include "../CoOrdinateAxis/CoOrdinateAxis.h"

#define SUCCESS 1
#define FAILURE 0

typedef int status;
typedef struct tagSIMPLELINEGRAPH
{
    LINE* Lines;
    LPCOORDINATEAXIS lpCoOrdinateAxis;
    int nSize;
}SIMPLELINEGRAPH,*LPSIMPLELINEGRAPH,*PSIMPLELINEGRAPH;

HWND Init(HINSTANCE hInstance,HWND hPrentWnd);

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int** pPoints,int m,int* nPSize,int scale,LPCOLORREF pColor);

status RelocateLines(LPSIMPLELINEGRAPH lpSimpleLineGraph,int** ppPoints,int* nPSize);

status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph);