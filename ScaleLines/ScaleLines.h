#pragma once

#include<Windows.h>
#include "../Error/Errors.h"
#include "../HetroTree/MemoryAllocator.h"

typedef unsigned long int unit;
typedef int status;
typedef unsigned long int Options;
typedef unsigned long int HSCALELINE;

#define SCLE_START_PT    0x1
#define SCLE_END_PT      0x2
#define SCLE_SPACE       0x4


#define SCLE_VSCROLL     0x8
#define SCLE_HSCROLL     0x10
#define SCLE_NOSCROLL    0x12

#define SCLE_INIT        10

typedef struct tagSCALELINE
{
    LPPOINT StartPoint;
    LPPOINT EndPoint;


    COLORREF ScaleColor;
    HBRUSH* hBrush;
    unit Mode;

    bst_node_t treelink;

    unit Space;    
}SCALELINE,*PSCALELINE,*LPSCALELINE;


key_t CALLBACK CreateScaleLines();
status SetScaleLines(key_t hScaleLine,Options option,const LPSCALELINE InScaleLines);
void DrawScaleLines(HDC hdc ,PPAINTSTRUCT ps,const key_t hScaleLine,Options option,int pos);
status CALLBACK DestoryScaleLines(bst_node_t* tree_node);