#include "CoOrdinateAxis.h"


LPCOORDINATEAXIS CALLBACK CreateCoOrdinateAxis()
{

        LPCOORDINATEAXIS lpCoOrdinate = (LPCOORDINATEAXIS) xcalloc(sizeof(COORDINATEAXIS));
        CHECK_ERROR( lpCoOrdinate == NULL,
        "Not able to allocate the CoOrdinate Axis",
        "CreateCoOrdinateAxis" );

        lpCoOrdinate->Origin = (LPPOINT) xcalloc(sizeof(POINT));
        lpCoOrdinate->XAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));
        lpCoOrdinate->YAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));

        
        // lpCoOrdinate->TitleX = (LPSTR) xcalloc(1,_coOrdinateAxis->XTitleLength);
        // lpCoOrdinate->TitleY = (LPSTR) xcalloc(1,_coOrdinateAxis->YTileLength);

        return lpCoOrdinate;
}

void Destroy(void* ptr)
{
    if(ptr != NULL)
    {
        free(ptr);
        ptr = NULL;
    }
}

void DestroyLables(LPSTR* lpLableBase)
{
    if(lpLableBase != NULL)
    {
        size_t length = sizeof(lpLableBase) / sizeof(lpLableBase[0]);
        for(int i=0;i<length;i++)
            Destroy(lpLableBase[i]);

        free(lpLableBase);
        lpLableBase = NULL;
    }
}

status SetCoOrdinateParamters(LPCOORDINATEAXIS lpCoOrdinate,const LPCOORDINATEAXIS InCoOrdinateAxis,options opt)
{
    if(InCoOrdinateAxis == NULL || lpCoOrdinate == NULL)
        return FAIL;

    if(opt & AX_INIT_X_AXIS )
    {
        CHECK_ERROR(
                    lpCoOrdinate->Origin == NULL || lpCoOrdinate->XAxisEnd == NULL,
                    "Not able to allocate memory to points",
                    "CreateCoOrdinateAxis()"
                    );
        lpCoOrdinate->Origin =  InCoOrdinateAxis->Origin;
        lpCoOrdinate->XAxisEnd   =  InCoOrdinateAxis->XAxisEnd;
    }

    if(opt & AX_INIT_Y_AXIS)
    {
        CHECK_ERROR(
                    lpCoOrdinate->Origin == NULL || lpCoOrdinate->YAxisEnd == NULL,
                    "Not able to allocate memory to points",
                    "CreateCoOrdinateAxis()"
                );
        
        lpCoOrdinate->Origin        =    InCoOrdinateAxis->Origin;
        lpCoOrdinate->YAxisEnd      =    InCoOrdinateAxis->YAxisEnd;
    }

    if(opt & AX_TITLE_X)
    {
        int nTitleLen = strnlen_s(InCoOrdinateAxis->TitleX,MAX_LABLE_NAME);
        lpCoOrdinate->TitleX = (CHAR*) xcalloc(nTitleLen);
        lpCoOrdinate->XTitleLength = nTitleLen;

        CHECK_ERROR(lpCoOrdinate->TitleX == NULL,
                    "Not able to allocate memory to title",
                    "CreateCoOrdinateAxis()"          
        );

        strcpy_s( lpCoOrdinate->TitleX
                    ,MAX_LABLE_NAME,
                    InCoOrdinateAxis->TitleX
            );
    }

    if(opt & AX_TITLE_Y)
    {   

        int nTitleLen = strnlen_s(InCoOrdinateAxis->TitleY,MAX_LABLE_NAME);
        lpCoOrdinate->TitleY = (CHAR*) xcalloc(nTitleLen);
        lpCoOrdinate->YTileLength = nTitleLen;

        CHECK_ERROR(lpCoOrdinate->TitleY == NULL,
                    "Not able to allocate memory for tile",
                    "CreateCoOrdinateAxis()"
                    );

        

        strcpy_s( lpCoOrdinate->TitleY,
                MAX_LABLE_NAME,
                InCoOrdinateAxis->TitleY
            );
    }

    if(opt & AX_LABLES)
    {

        // if(opt & AX_UPDATE)
        //     DestroyLables(lpCoOrdinate->lables);

        // size_t length = sizeof(InCoOrdinateAxis->lables) / sizeof(InCoOrdinateAxis->lables[0]);
        // lpCoOrdinate->lables = (LPSTR*) xcalloc(length,sizeof(LPSTR));
        // CHECK_ERROR(lpCoOrdinate->lables == NULL,
        //         "Not able allocate the message",
        //         "CreateCoOrdinateAxis()"
        //     );
        
        // for(int i=0;i<length;i++)
        // {
        //     size_t lableLength = strnlen_s(InCoOrdinateAxis->lables[i],MAX_LABLE_NAME);
        //     lpCoOrdinate->lables[i] = (PCHAR)xcalloc(lableLength,sizeof(CHAR));
        //     CHECK_ERROR(lpCoOrdinate->lables[i] == NULL
        //                 ,"Not able to allocate the string"
        //                 ,"CreateCoOrdinateAxis()"
        //     );
        // }
    }

    if(opt & AX_SCALE)
    {
        lpCoOrdinate->scale = InCoOrdinateAxis->scale;
    }

    return SUCCESS;

}

status GetAxis(Axis axis,LPAXIS pAxis,const LPCOORDINATEAXIS InCoOrdinateAxis)
{
    if(axis == XAXIS)
    {
        pAxis->start.x = InCoOrdinateAxis->Origin->x;
        pAxis->start.y = InCoOrdinateAxis->Origin->y;

        pAxis->end.x = InCoOrdinateAxis->XAxisEnd->x;
        pAxis->end.y = InCoOrdinateAxis->XAxisEnd->y;
    }

    else if(axis == YAXIS)
    {
        pAxis->start.x = InCoOrdinateAxis->Origin->x;
        pAxis->start.y = InCoOrdinateAxis->Origin->y;

        pAxis->end.x = InCoOrdinateAxis->YAxisEnd->x;
        pAxis->end.y = InCoOrdinateAxis->YAxisEnd->y;
    }

    return SUCCESS;
}

status GetOrigin(const LPCOORDINATEAXIS InOrdinateAxis,LPPOINT OutOrigin)
{
    (OutOrigin)->x = InOrdinateAxis->Origin->x;
    (OutOrigin)->y = InOrdinateAxis->Origin->y;

    return SUCCESS;
}

/*
        TODO :

            Optimize the drawing algorithm for CoOrdinateAxis VSCROLL and HSCROLL
*/

status DrawCoOrdinateAxis(HDC hdc,PPAINTSTRUCT ps,const LPCOORDINATEAXIS InOrdinateAxis,scrollOption option,int vPos,int hPos)
{
    LPPOINT O   =   InOrdinateAxis->Origin;
    LPPOINT OX  =   InOrdinateAxis->XAxisEnd;
    LPPOINT OY  =   InOrdinateAxis->YAxisEnd;
    
    POINT P1;
    POINT P2;
    POINT P3;

    memcpy(&P1,O,sizeof(POINT));
    memcpy(&P2,OY,sizeof(POINT));
    memcpy(&P3,OX,sizeof(POINT));

    int PPI = GetDeviceCaps(hdc,LOGPIXELSY);

    if(option & AX_VSCROLL)
    {

        /*
            1. Get the rcPaint RECT.

            2. Let P1 and P2 be two points of the Y-axis and P1 is near to Origin.

            so at any time (WM_PAINT or WM_VSCROLL) P1 and P2 will follow the following rule

            if rcPaint.bottom < Origin.y (considering scroll position)
                P1.y = rcPaint.bottom;
            if rcPaint.bottom >= Origin.y (considering scroll position)
                P1.y = Origin.Y;

            if rcPaint.top <= YAxisEnd.y (considering the scroll position)
                P2.y = YAxisEnd.y;
            if rcPaint.top > YAxisEnd.y (considering the scroll position)
                P2.y = rcPaint.top

        */
        if(ps->rcPaint.top <= (OY->y + (PPI * vPos)))
        {
            // P2.x = OY->x;
            P2.y = OY->y + (PPI * vPos);
        }

        else if(ps->rcPaint.top > (OY->y))
        {
            // P2.x = OY->x;
            P2.y = OY->y;
        }

        MoveToEx(hdc,P2.x,P2.y,NULL);
        LineTo(hdc,O->x,O->y);

        /*
                1. Get the rcPaint RECT

                2. The Drawing of the X-Axis will follow the following rule (for WM_PAINT or WM_VSCROLL)

                3. if rcPaint bottom >= O->y (cosidering scroll pos) then draw the X-Axis
        
        */

        

        // if(vPos == 0) // ps->rcPaint.bottom >= ((O->y + (PPI * pos)))
        // {
        //     MoveToEx(hdc,O->x,O->y + (PPI * vPos),NULL);
        //     LineTo(hdc,OX->x,OX->y + (PPI * vPos));

        //     SetTextAlign(hdc,TA_CENTER);
        //     TextOut(hdc,InOrdinateAxis->XAxisEnd->x / 2,InOrdinateAxis->Origin->y + AX_OFFSET,InOrdinateAxis->TitleX,InOrdinateAxis->XTitleLength);

        //     // MessageBox((HWND)NULL,"ASdads","Asdad",MB_ICONERROR);
        // }

        

    }
    if(option & AX_HSCROLL)
    {

        // if(ps->rcPaint.left <= O->x)
        // {
        //     P1.x = O->x;
        // }
        // else if(ps->rcPaint.left > O->x)
        // {
        //     P1.x = ps->rcPaint.left;
        // }

        if(ps->rcPaint.right <= (OX->x - hPos))
        {
            P3.x = ps->rcPaint.right;
        }
        else if(ps->rcPaint.right > (OX->x - hPos))
        {
            P3.x = OX->x - hPos;
        }

        MoveToEx(hdc,O->x,O->y,NULL);
        LineTo(hdc,P3.x,P3.y);

        SetTextAlign(hdc,TA_CENTER);
        TextOut(hdc,P3.x / 2,InOrdinateAxis->Origin->y + AX_OFFSET,InOrdinateAxis->TitleX,InOrdinateAxis->XTitleLength);

        // static CHAR buff[500];
        // wsprintf(buff,"P1.x : %d P3.x : %d hPos : %d OX->x : %d",O->x,P3.x,hPos,Ox);
        // MessageBox((HWND)NULL,buff,"WM_SIZE",MB_ICONERROR);
    }

    if(option & AX_NOSCROLL)
    {
        MoveToEx(hdc,InOrdinateAxis->Origin->x,InOrdinateAxis->Origin->y,NULL);
        LineTo(hdc,InOrdinateAxis->XAxisEnd->x,InOrdinateAxis->XAxisEnd->y);

        MoveToEx(hdc,InOrdinateAxis->Origin->x,InOrdinateAxis->Origin->y,NULL);
        LineTo(hdc,InOrdinateAxis->YAxisEnd->x,InOrdinateAxis->YAxisEnd->y);
    }
    return SUCCESS;
}

status CALLBACK DestoryCoOrdinateAxis(LPCOORDINATEAXIS _coOrdinateAxis)
{
    if(_coOrdinateAxis != NULL)
    {
        Destroy(_coOrdinateAxis->Origin);
        Destroy(_coOrdinateAxis->XAxisEnd);
        Destroy(_coOrdinateAxis->YAxisEnd);

        Destroy(_coOrdinateAxis->TitleX);
        Destroy(_coOrdinateAxis->TitleY);
        // DestroyLables(_coOrdinateAxis->lables);

        _coOrdinateAxis->XTitleLength = 0;
        _coOrdinateAxis->YTileLength  = 0;

        free(_coOrdinateAxis);
        _coOrdinateAxis->Origin = NULL;
        _coOrdinateAxis->XAxisEnd = NULL;
        _coOrdinateAxis->YAxisEnd = NULL;
        _coOrdinateAxis->TitleX = NULL;
        _coOrdinateAxis->TitleY = NULL;
        // _coOrdinateAxis->lables = NULL;
        _coOrdinateAxis = NULL;

         return SUCCESS;
    }
    
    return FAIL;

}

// status IsLineInRect(LPPOINT start,LPPOINT end,PRECT rcOverlapped)
// {
    
// }

// -------------------------------------------------------------------------
//                                 .       

// ------------------------------             --------------------

//                                         .
// -------------------------------------------------------------------------