#include "Line.h"

LPLINE CreateLine()
{
    LPLINE lpLine = (LPLINE) calloc(1,sizeof(LINE));
    CHECK_ERROR(lpLine == NULL,"Not able to allocate the Line","CreateLine");
    return lpLine;
}

status SetLine(LPLINE lpLine,const LPLINE InLine,option options)
{
    if(options & LNE_POINTS)
    {
        lpLine->pPoints = InLine->pPoints;
        CHECK_ERROR(lpLine->pPoints == NULL,
            "Failed to allocate points",
            "SetLine()"
        );
        lpLine->nSize = InLine->nSize;
    }

    if(options & LNE_COLOR)
    {
        lpLine->colLine = InLine->colLine;
    }

    return SUCCESS;
}


status DrawLine(HDC hdc,LPPAINTSTRUCT ps,LPLINE line,LPSCROLLINFO vScroll,LPSCROLLINFO hScroll)
{
    
    // for(int i=0;i<line->nSize;i++)
    // {
    //     // static CHAR buff[200];
    //     // // wsprintf(buff,"length : %d ",line->nSize);
    //     // wsprintf(buff,"point x : %d y : %d",line->pPoints[i].x,line->pPoints[i].y);
    //     // MessageBox((HWND)NULL,buff,"Points",MB_ICONERROR);
    // }
    Polyline(hdc,line->pPoints,line->nSize);

    return SUCCESS;
}

status DestoryLine(LPLINE lpLine)
{
    if(lpLine->pPoints == NULL)
        return SUCCESS;
    
    free(lpLine->pPoints);
    free(lpLine);

    lpLine->pPoints = NULL;
    lpLine = NULL;

    return SUCCESS;
}
