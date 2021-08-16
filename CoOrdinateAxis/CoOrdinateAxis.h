#pragma once

#include<Windows.h>

#include "../ScaleLines/ScaleLines.h"
#include "../Errors.h"

#define SUCCESS            0
#define FAIL               1

#define AX_OFFSET         30

#define XAXIS              0
#define YAXIS              1

#define AX_INIT_X_AXIS     2
#define AX_INIT_Y_AXIS     4
#define AX_TITLE_X         8
#define AX_TITLE_Y         16
#define AX_LABLES          32
#define AX_UPDATE          64
#define AX_SCALE           128
#define AX_VSCROLL         256
#define AX_HSCROLL         512
#define AX_NOSCROLL        1024

#define MAX_LABLE_NAME  200

#define LOGICAL_ORIGINE_X 30
#define LOGICAL_ORIGINE_Y 30

typedef int Axis;
typedef unsigned long int options;
typedef options scrollOption;
typedef int status;

typedef struct tagAXIS
{
    POINT start;
    POINT end;
}AXIS,*PAXIS,*LPAXIS;

typedef struct tagCOORDINATEAXIS
{
    LPPOINT Origin;

    LPPOINT XAxisEnd;
    LPPOINT YAxisEnd;

    CHAR* TitleX;
    size_t XTitleLength;

    CHAR* TitleY;
    size_t YTileLength;

    unsigned long int  scale;

}COORDINATEAXIS,*PCOORDINATEAXIS,*LPCOORDINATEAXIS;


LPCOORDINATEAXIS CreateCoOrdinateAxis();
void Destroy(void* ptr);
void DestroyLables(LPSTR* lpLableBase);

status SetCoOrdinateParamters(LPCOORDINATEAXIS lpCoOrdinate,const LPCOORDINATEAXIS InCoOrdinateAxis,options opt);



status GetAxis(Axis axis,LPAXIS pAxis,const LPCOORDINATEAXIS InCoOrdinateAxis);
status GetOrigin(const LPCOORDINATEAXIS InOrdinateAxis,LPPOINT OutOrigin);

status DrawCoOrdinateAxis(HDC hdc,PPAINTSTRUCT ps,const LPCOORDINATEAXIS InOrdinateAxis,scrollOption option,int vPos,int hPos);

status DestoryCoOrdinateAxis(LPCOORDINATEAXIS _coOrdinateAxis);
