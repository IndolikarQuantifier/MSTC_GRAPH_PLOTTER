#pragma once
#include<Windows.h>

#include "../CoOrdinateAxis/CoOrdinateAxis.h"
#include "../Bars/Bars.h"
#include "../Errors.h"

#define GAP 25
#define BARWDTH 96
#define GENESIS_BAR 50
#define OFFSET 96


#define BRGRPH_SAVE 101

typedef unsigned long int Gap;
typedef unsigned long int BarWdth;
typedef unsigned long int GenesisBar;

typedef struct tagSIMPLEBARGRAPH
{
    Gap gap;
    BarWdth barWdth;
    GenesisBar genesisBar;

    LPCOORDINATEAXIS lpCoOrdinateAxis;
    LPVECBARS lpVecBars;
    LPSCALELINE lpScaleLine;

    unit maxHeight;

}SIMPLEBARGRAPH,*PSIMPLEBARGRAPH,*LPSIMPLEBARGRAPH;


HWND Init(HINSTANCE hInstance,HWND hParentWnd);
int CreateBarGraph(HWND hWnd,LPSIMPLEBARGRAPH* lpSimpleBarGraph,unit* Heights,LPCOLORREF lpColor,unit scale,size_t iSize,const LPSTR xTitle,const LPSTR yTitle,const CHAR** lables);
// void UpdateBarGraph()

// status CreteCoCordinateAxis()

status CreateDataBars(LPSIMPLEBARGRAPH lpSimpleBarGraph,unit* barData,LPCOLORREF lpColor,const CHAR** lables,size_t iSize);
status ReLocateBars(LPSIMPLEBARGRAPH lpSimpleBarGraph);

void DrawSimpleBarGraph(HDC hdc,LPPAINTSTRUCT ps,LPSIMPLEBARGRAPH lpSimpleBarGraph);
void DestroySimpleBarGraph(LPSIMPLEBARGRAPH lpSimpleBarGraph);