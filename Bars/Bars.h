#pragma once

#include<Windows.h>
#include "../Error/Errors.h"
#include "../GenericDCLL/GDCLL.h"
#include "../HetroTree/MemoryAllocator.h"

#define DEFAULT_WIDTH       100


#define MAX_LABLE_LENGTH    20
#define LBL_OFFSET          10

#define INIT_WIDTH          3
#define INIT_HEIGHT         4

#define BR_WIDTH            1
#define BR_WIDTH_MULTI      2
#define BR_WIDTH_RANGE      4

#define BR_HEIGHT           8
#define BR_HEIGHT_MULTI     16
#define BR_HEIGHT_RANGE     32

#define BR_VSCROLL          64
#define BR_HSCROLL          128
#define BR_NOSCROLL         256
#define BR_SIZE             512

typedef unsigned long int CNT;
typedef int status;
typedef unsigned long int GetFlags;
typedef unsigned long int EditFlags;
typedef unsigned long int InitFlags;
typedef unsigned long int unit;
typedef unsigned long int scrollOption;

typedef struct tagDISPLAYBARATTR
{
    RECT rect;

    NODE link;

}DISPLAYBARATTR,*PDISPLAYBARATTR,*LPDISPLAYBARATTR;
typedef struct tagEXTRA
{
    unit *arr;
    size_t iSize;

    int start;
    int end;

}EXTRA,*PEXTRA,*NPEXTRA,*LPEXTRA;

typedef struct tagBAR
{
    unit _width;
    unit _height;

    RECT _box;
    COLORREF _color;

    const CHAR* lable;

}BAR,*PBAR,*NPBAR,*LPBAR;

typedef struct tagVECTORBARS
{
    LPBAR lpBar;
    HBRUSH* hBrushes;

    LPDCLL DisplayBuffer;

    bst_node_t treelink;
    size_t iSize;
    
}VECBARS,*PVECBARS,*NPVECBARS,*LPVECBARS;

LPDISPLAYBARATTR CreateDisplayBarAttr(LONG left,LONG right);
status_t DestroyDisplayBarAttr(LPNODE rm_node);

// status InitBars(LPVECBARS _lpVecBars,const PRECT InlpRect,const CHAR** lables,const LPCOLORREF InlpColor,size_t _totRect);

status InitBars(key_t hVecBars,const PRECT InlpRect,const CHAR** lables,const LPCOLORREF InlpColor,size_t _totRect);

key_t CALLBACK CreateBars(CNT _cnt);

status UpdateHeight(key_t hVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra);
status UpdateWidth(key_t hVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra);
status SetDisplayRectBuffer(key_t hVecBars,int nSize);

LPEXTRA SetExtras(unit* InArr,size_t _length,int _start,int _end);
status_t GetVectorBarAttributes(key_t hVecBars,GetFlags options,LPVECBARS outVecBars);
// LPEXTRA SetColor(uint* _arr,size_t _length,int start,int end);


status DrawBars(HDC hdc,PAINTSTRUCT *ps,key_t hVecBars,scrollOption option,int vPos,LPSCROLLINFO hScrollInfo);
status CALLBACK DestoryBars(bst_node_t* tree_node);
status DestroyExtra(LPEXTRA _lpExtra);
