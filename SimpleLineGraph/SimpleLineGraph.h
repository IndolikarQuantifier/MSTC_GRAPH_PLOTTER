#pragma once

#include<Windows.h>
#include<stdlib.h>
#include "../Lines/Line.h"
#include "../CoOrdinateAxis/CoOrdinateAxis.h"
#include "../Error/Errors.h"

#define LINE_GRAPH_SAVE 10
#define DEFAULT_GENESIS 96

typedef int status;
typedef struct tagSIMPLELINEGRAPH
{
    int genesisPoint;
    int maxPoints;
    int pointGap;
    LINE* Lines;
    LPCOORDINATEAXIS lpCoOrdinateAxis;
    int nSize;
}SIMPLELINEGRAPH,*LPSIMPLELINEGRAPH,*PSIMPLELINEGRAPH;

HWND Init(HINSTANCE hInstance,HWND hPrentWnd);

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int** pPoints,int m,int* nPSize,int scale,LPCOLORREF pColor);

status RelocateLines(LPSIMPLELINEGRAPH lpSimpleLineGraph,int** ppPoints,int* nPSize);

status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph);