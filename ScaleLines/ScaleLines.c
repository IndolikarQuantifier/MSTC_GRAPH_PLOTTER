#include "ScaleLines.h"


LPSCALELINE CreateScaleLines()
{
    LPSCALELINE lpScaleLine = (LPSCALELINE) calloc(1,sizeof(SCALELINE));
    lpScaleLine->StartPoint = (LPPOINT) calloc(1,sizeof(POINT));
    lpScaleLine->EndPoint   = (LPPOINT) calloc(1,sizeof(POINT));

    lpScaleLine->ScaleColor = RGB(236,228,228);
    lpScaleLine->hBrush = (HBRUSH*) calloc(1,sizeof(HBRUSH));

    lpScaleLine->hBrush[0] = CreateSolidBrush(lpScaleLine->ScaleColor);

    CHECK_ERROR(lpScaleLine == NULL,
        "Not able to allocate memory to scalelines",
        "CreateScaleLines"
    );

    return lpScaleLine;
}

status SetScaleLines(LPSCALELINE _lpScaleLine,Options option,const LPSCALELINE InScaleLines)
{
    if(_lpScaleLine == NULL)
        return FAIL;

    if(option & SCLE_START_PT)
    {
        _lpScaleLine->StartPoint = InScaleLines->StartPoint;
        // _lpScaleLine->StartPoint->y = InScaleLines->StartPoint->y;
    }

    if(option & SCLE_END_PT)
    {
        _lpScaleLine->EndPoint   = InScaleLines->EndPoint;
        // _lpScaleLine->EndPoint->y   = InScaleLines->EndPoint->y;
    }
    
    if(option & SCLE_SPACE)
    {
        _lpScaleLine->Space = InScaleLines->Space;
    }

    return SUCCESS;
}

void DrawScaleLines(HDC hdc ,PPAINTSTRUCT ps,const LPSCALELINE _lpScaleLine,Options option,int pos)
{

    static CHAR buff[100];
    static TEXTMETRIC tm;
    static CyChar;

    GetTextMetrics(hdc,&tm);
    CyChar = tm.tmHeight + tm.tmExternalLeading;

    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);

    LPPOINT O   =   _lpScaleLine->StartPoint;
    // LPPOINT OX  =   InOrdinateAxis->XAxisEnd;
    LPPOINT OY  =   _lpScaleLine->EndPoint;

    POINT P1 = {0,0};
    POINT P2 = {0,0};

    int nLables = pos * _lpScaleLine->Space;

    if(option & SCLE_VSCROLL)
    {
    
        P1.x = _lpScaleLine->StartPoint->x;
        P1.y = _lpScaleLine->StartPoint->y;

        if(ps->rcPaint.top <= (OY->y + (RESY * pos)))
        {
            // P2.x = OY->x;
            P2.y = OY->y + (RESY * pos);
        }

        else if(ps->rcPaint.top > (OY->y))
        {
            // P2.x = OY->x;
            P2.y = OY->y;
            // MessageBox((HWND)NULL,"ps->rcPaint.top > (OY->y)","ScaleLine",MB_ICONERROR);
        }

    }

    if(option & SCLE_HSCROLL)
    {
       
    }

    for(int i = P1.y;i>=P2.y;i -= RESY,nLables += _lpScaleLine->Space)
    {
            // SelectObject(hdc,_lpScaleLine->hBrush[0]);

            // P1.x will change for Horizontal Scroll
            MoveToEx(hdc,P1.x,i,NULL);
            LineTo(hdc,ps->rcPaint.right,i);

            SetTextAlign(hdc,TA_RIGHT);

            int nCnt = wsprintf(buff,"%d",nLables);
            TextOut(hdc,P1.x-10,i - CyChar,buff,nCnt);
    }
    
}

status DestoryScaleLines(LPSCALELINE lpScaleLine)
{
    if(lpScaleLine == NULL)
        return FAIL;
        
    free(lpScaleLine->StartPoint);
    free(lpScaleLine->EndPoint);
    DeleteObject(lpScaleLine->hBrush[0]);
    free(lpScaleLine->hBrush);
    free(lpScaleLine);
    
    lpScaleLine->StartPoint = NULL;
    lpScaleLine->EndPoint   = NULL;
    lpScaleLine->hBrush     = NULL;
    lpScaleLine             = NULL;

    return SUCCESS;
}