#include "CoOrdinateAxis.h"



key_t CALLBACK CreateCoOrdinateAxis()
{

        LPCOORDINATEAXIS lpCoOrdinate = NULL;//(LPCOORDINATEAXIS) xcalloc(sizeof(COORDINATEAXIS));
        key_t CoordinateKey;
        GraphResourceAlloc(COORDINATEAXIS,treelink,sizeof(COORDINATEAXIS),lpCoOrdinate,CoordinateKey,DestoryCoOrdinateAxis,NULL);

        CHECK_ERROR( lpCoOrdinate == NULL,
        "Not able to allocate the CoOrdinate Axis",
        "CreateCoOrdinateAxis" );

        lpCoOrdinate->Origin = (LPPOINT) xcalloc(sizeof(POINT));
        lpCoOrdinate->XAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));
        lpCoOrdinate->YAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));

        
        // lpCoOrdinate->TitleX = (LPSTR) xcalloc(1,_coOrdinateAxis->XTitleLength);
        // lpCoOrdinate->TitleY = (LPSTR) xcalloc(1,_coOrdinateAxis->YTileLength);

        return CoordinateKey;
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

status SetCoOrdinateParamters(key_t hCoOrdinateAxis,const LPCOORDINATEAXIS InCoOrdinateAxis,options opt)
{
    bst_node_t* treenode = NULL;
    GetGraphResource(hCoOrdinateAxis,&treenode);

    LPCOORDINATEAXIS lpCoOrdinate = CONTAINER_OF(treenode,COORDINATEAXIS,treelink);

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

status GetAxis(Axis axis,LPAXIS pAxis,const key_t hCoOrdinateAxis)
{
    bst_node_t* treenode = NULL;
    GetGraphResource(hCoOrdinateAxis,&treenode);

    LPCOORDINATEAXIS lpCoOrdinate = CONTAINER_OF(treenode,COORDINATEAXIS,treelink);

    if(axis == XAXIS)
    {
        pAxis->start.x = lpCoOrdinate->Origin->x;
        pAxis->start.y = lpCoOrdinate->Origin->y;

        pAxis->end.x = lpCoOrdinate->XAxisEnd->x;
        pAxis->end.y = lpCoOrdinate->XAxisEnd->y;
    }

    else if(axis == YAXIS)
    {
        pAxis->start.x = lpCoOrdinate->Origin->x;
        pAxis->start.y = lpCoOrdinate->Origin->y;

        pAxis->end.x = lpCoOrdinate->YAxisEnd->x;
        pAxis->end.y = lpCoOrdinate->YAxisEnd->y;
    }

    return SUCCESS;
}

status GetOrigin(const key_t hCoOrdinateAxis,LPPOINT OutOrigin)
{
    bst_node_t* treenode = NULL;
    GetGraphResource(hCoOrdinateAxis,&treenode);

    LPCOORDINATEAXIS lpCoOrdinate = CONTAINER_OF(treenode,COORDINATEAXIS,treelink);

    (OutOrigin)->x = lpCoOrdinate->Origin->x;
    (OutOrigin)->y = lpCoOrdinate->Origin->y;

    return SUCCESS;
}

status GetScale(const key_t hCoOrdinateAxis,LPCOORDINATEAXIS OutCoOrdinateAxis)
{
    bst_node_t* treenode = NULL;
    GetGraphResource(hCoOrdinateAxis,&treenode);

    LPCOORDINATEAXIS lpCoOrdinate = CONTAINER_OF(treenode,COORDINATEAXIS,treelink);

    OutCoOrdinateAxis->scale = lpCoOrdinate->scale;

    return SUCCESS;
}

/*
        TODO :

            Optimize the drawing algorithm for CoOrdinateAxis VSCROLL and HSCROLL
*/

status DrawCoOrdinateAxis(HDC hdc,PPAINTSTRUCT ps,const key_t hCoOrdinateAxis,scrollOption option,int vPos,int hPos,int wndWidth)
{

    bst_node_t* treenode = NULL;
    GetGraphResource(hCoOrdinateAxis,&treenode);

    LPCOORDINATEAXIS lpCoOrdinate = CONTAINER_OF(treenode,COORDINATEAXIS,treelink);

    LPPOINT O   =   lpCoOrdinate->Origin;
    LPPOINT OX  =   lpCoOrdinate->XAxisEnd;
    LPPOINT OY  =   lpCoOrdinate->YAxisEnd;
    
    POINT P1;
    POINT P2;
    POINT P3;

    memcpy(&P1,O,sizeof(POINT));
    memcpy(&P2,OY,sizeof(POINT));
    memcpy(&P3,OX,sizeof(POINT));

    int PPI = GetDeviceCaps(hdc,LOGPIXELSY);

    // static CHAR buff[500];
    // wsprintf(buff,"OX.x : %d",OX->x);
    // MessageBox((HWND)NULL,buff,"WM_SIZE",MB_ICONERROR);

    if(option & AX_VSCROLL)
    {

        /*
    5        1. Get the rcPaint RECT.

            2. Let P1 and P2 be two points of the Y-axis and P1 is near to Origin.
    b
            so at any time (WM_PAINT or WM_VSCROLL) P1 and P2 will follow the following rule

            if rcPaint.bottom < Origin.y (considering scroll position)
                P1.y = rcPaint.bottom;
            if rcPaint.bottom >= Origin.y (considering scroll position)
    a            P1.y = Origin.Y;

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
        TextOut(hdc,wndWidth / 2,lpCoOrdinate->Origin->y + AX_OFFSET,lpCoOrdinate->TitleX,lpCoOrdinate->XTitleLength);

        // static CHAR buff[500];
        // wsprintf(buff,"P3.x : %d hPos : %d rcPaint : %d OX->x : %d",P3.x,hPos,ps->rcPaint.right,OX->x);
        // MessageBox((HWND)NULL,buff,"WM_SIZE",MB_ICONERROR);
    }

    if(option & AX_NOSCROLL)
    {
        MoveToEx(hdc,lpCoOrdinate->Origin->x,lpCoOrdinate->Origin->y,NULL);
        LineTo(hdc,lpCoOrdinate->XAxisEnd->x,lpCoOrdinate->XAxisEnd->y);

        MoveToEx(hdc,lpCoOrdinate->Origin->x,lpCoOrdinate->Origin->y,NULL);
        LineTo(hdc,lpCoOrdinate->YAxisEnd->x,lpCoOrdinate->YAxisEnd->y);
    }
    return SUCCESS;
}

status CALLBACK DestoryCoOrdinateAxis(bst_node_t* tree_node)
{
    LPCOORDINATEAXIS _coOrdinateAxis = NULL;

    _coOrdinateAxis = CONTAINER_OF(tree_node,COORDINATEAXIS,treelink);

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