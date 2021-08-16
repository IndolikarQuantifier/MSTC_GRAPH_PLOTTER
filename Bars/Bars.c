#include "Bars.h"
#include<stdio.h>

LPVECBARS CreateBars(CNT _cnt)
{
    LPVECBARS lpVecBars = NULL;
    lpVecBars = (LPVECBARS) realloc(lpVecBars,sizeof(VECBARS));
    CHECK_ERROR(lpVecBars == NULL,"Not able to allocate the vector of bars","CreateBars()");

    lpVecBars->lpBar = (LPBAR) calloc(_cnt,sizeof(BAR));
    lpVecBars->hBrushes = (HBRUSH*) calloc(_cnt,sizeof(HBRUSH));
    lpVecBars->iSize = _cnt;
    lpVecBars->DisplayRect = NULL;

    return lpVecBars;
};

/*

        Used to Give the Inital Values of Bars in the form of rects
        so that all other parameters are set by this InitBars()
        
        This accepts the three paramters

        1. The Vector which contains the bars
        2. The array of RECTS that should be the bars of graph
        3. Size of that array
*/ 

status InitBars(LPVECBARS _lpVecBars,const PRECT InlpRect,const CHAR** lables,const LPCOLORREF InlpColor,size_t _totRect)
{

    for(size_t i =0;i<_totRect;i++)
    {
        _lpVecBars->lpBar[i]._box = InlpRect[i];
        _lpVecBars->lpBar[i]._width = InlpRect[i].left - InlpRect[i].right;
        _lpVecBars->lpBar[i]._height = InlpRect[i].bottom - InlpRect[i].top;
        _lpVecBars->lpBar[i]._color = InlpColor[i];
        _lpVecBars->lpBar[i].lable = lables[i];
        _lpVecBars->hBrushes[i] = CreateSolidBrush(InlpColor[i]);
    }

    return SUCCESS;
}

status UpdateHeight(LPVECBARS _lpVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra)
{
    if(options & BR_HEIGHT)
    {
        for(int i=0;i<_lpVecBars->iSize;i++)
        {
            _lpVecBars->lpBar[i]._height = value;
            _lpVecBars->lpBar[i]._box.top = _lpVecBars->lpBar[i]._box.bottom - value;
        }
    }

    if(options & BR_HEIGHT_MULTI)
    {

        CHECK_ERROR(InExtra == NULL,"Provide valid InExtra info","UpdateHeight()");

        int start = max(0,InExtra->start);
        int end = min(_lpVecBars->iSize,InExtra->end);

        for(int i=start;i<end;i++)
        {
            _lpVecBars->lpBar[i]._height = InExtra->arr[i];
            _lpVecBars->lpBar[i]._box.top = _lpVecBars->lpBar[i]._box.bottom - InExtra->arr[i];
        }

    }

    return SUCCESS;
}

status UpdateWidth(LPVECBARS _lpVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra)
{

    if(options & BR_WIDTH)
    {
        for(int i=0; i < _lpVecBars->iSize; i++)
        {
            _lpVecBars->lpBar[i]._width = value;
            _lpVecBars->lpBar[i]._box.right = _lpVecBars->lpBar[i]._box.left + value;
        }
    }

    if(options & BR_WIDTH_MULTI)
    {   
        CHECK_ERROR(InExtra == NULL,"Provide the valid InExtra info","UpdateWidth()");

        int start,end;

        start = max(0,InExtra->start);
        end = min(_lpVecBars->iSize,InExtra->end);
            
        for(int i=start; i<end; i++)
        {
            _lpVecBars->lpBar[i]._width = InExtra->arr[i];
            _lpVecBars->lpBar[i]._box.right = _lpVecBars->lpBar[i]._box.left + InExtra->arr[i];
        }
    }

    
    return SUCCESS;
}

/*
        TODO :

                Optimise the BAR's VSCROLL and HSCROLL
*/

status DrawBars(HDC hdc,PAINTSTRUCT *ps,LPVECBARS _lpVecBars,scrollOption option,int vPos,LPSCROLLINFO hScrollInfo)
{
    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);
    RECT rcTmp;
    RECT rcOverlap;

    PRECT pRect = _lpVecBars->DisplayRect;

    CopyRect(&rcTmp,&(_lpVecBars->lpBar[0]._box));

    int width = rcTmp.right - rcTmp.left;
    int gap = _lpVecBars->lpBar[1]._box.right - rcTmp.left;


    int nSize = (_lpVecBars->iSize <= (hScrollInfo->nPos + hScrollInfo->nPage)
                                    ?
                            _lpVecBars->iSize
                                    :
                  hScrollInfo->nPos + hScrollInfo->nPage
                );

    if(option & BR_HSCROLL)
    {
        for(int i = hScrollInfo->nPos,j=0;
                j < hScrollInfo->nPage && i < _lpVecBars->iSize;
                i++,j++
            )
        {

            CopyRect(&(_lpVecBars->DisplayRect[j]),&(_lpVecBars->lpBar[j]._box));

            _lpVecBars->DisplayRect[j].top = _lpVecBars->lpBar[i]._box.top;

            _lpVecBars->DisplayRect[j].bottom = _lpVecBars->lpBar[i]._box.bottom;
            
        }
    }

    if(option & BR_VSCROLL)
    {

        /*
        

        */


        for(int i=hScrollInfo->nPos,j=0;i<nSize;i++,j++)
        {
            _lpVecBars->DisplayRect[j].top = 
                _lpVecBars->DisplayRect[j].top + (RESY * vPos) < 0 ? 0 : _lpVecBars->DisplayRect[j].top + (RESY * vPos);
            // _lpVecBars->DisplayRect[j].bottom = _lpVecBars->DisplayRect[j].bottom + (RESY * vPos);
            // MessageBox((HWND)NULL,"VSCROLL","asda",MB_ICONERROR);
            if(IntersectRect(&rcOverlap,&(ps->rcPaint),&(_lpVecBars->DisplayRect[j])))
            {
                Rectangle(hdc,_lpVecBars->DisplayRect[j].left,_lpVecBars->DisplayRect[j].top,_lpVecBars->DisplayRect[j].right,_lpVecBars->DisplayRect[j].bottom);
                FillRect(hdc,&(_lpVecBars->DisplayRect[j]),_lpVecBars->hBrushes[i]);
            }

            int Xmid = (    _lpVecBars->DisplayRect[j].left
                                            + 
                                _lpVecBars->DisplayRect[j].right
                        ) / 2;
            SetTextAlign(hdc,TA_CENTER);
            TextOut(hdc,Xmid,_lpVecBars->lpBar[i]._box.bottom + LBL_OFFSET,_lpVecBars->lpBar[i].lable,strnlen_s(_lpVecBars->lpBar[i].lable,MAX_LABLE_LENGTH));
        }
    }

    

    if(option & BR_NOSCROLL)
    {
        for(int i=0;i<_lpVecBars->iSize;i++)
        {
            RECT rect = _lpVecBars->lpBar[i]._box;
            Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
            FillRect(hdc,&rect,_lpVecBars->hBrushes[i]);
        }
        
    }

    return SUCCESS;
}

status SetDisplayRectBuffer(LPVECBARS _lpVecBars,int nSize)
{
    // if(_lpVecBars->DisplayRect != NULL)
    // {
    //     free(_lpVecBars->DisplayRect);
    //     _lpVecBars->DisplayRect = NULL;
    // }
    PRECT pRect = (PRECT) realloc(_lpVecBars->DisplayRect,nSize * sizeof(RECT));
    _lpVecBars->DisplayRect = pRect;
    ZeroMemory(pRect,nSize * sizeof(RECT));

    // for(int i=0;i<nSize;i++)
    // {
    //     static CHAR buff[100];
    //     wsprintf(buff,"left : %d top : %d right : %d bottom : %d",
    //                 _lpVecBars->DisplayRect[i].left,
    //                 _lpVecBars->DisplayRect[i].top,
    //                 _lpVecBars->DisplayRect[i].right,
    //                 _lpVecBars->DisplayRect[i].bottom
    //             );
    //     MessageBox((HWND)NULL,buff,"PRECT",MB_ICONERROR);
    // }

    return SUCCESS;
}

LPEXTRA SetExtras(unit* InArr,size_t _length,int _start,int _end)
{
    LPEXTRA lpExtra = (LPEXTRA) calloc(1,sizeof(EXTRA));

    CHECK_ERROR(lpExtra == NULL,"Not able to create Extra","SetExtra");

    lpExtra->arr = InArr;
    lpExtra->iSize = _length;
    lpExtra->start = _start;
    lpExtra->end = _end;

    return lpExtra;
}

status DestoryBars(LPVECBARS _lpVecBars)
{
    if(_lpVecBars != NULL)
        return FAIL;

    for(int i=0;i<_lpVecBars->iSize;i++)
    {
        free(&(_lpVecBars->lpBar[i]._box));
        free(&(_lpVecBars->lpBar[i]._color));
        DeleteObject(_lpVecBars->hBrushes[i]);
    }

    free(_lpVecBars->DisplayRect);
    free(_lpVecBars->lpBar);
    free(_lpVecBars->hBrushes);
    free(_lpVecBars);

    _lpVecBars->lpBar = NULL;
    _lpVecBars        = NULL;
    _lpVecBars->DisplayRect = NULL;

    return SUCCESS;
}

status DestroyExtra(LPEXTRA _lpExtra)
{
    if(_lpExtra == NULL)
        return FAIL;

    free(_lpExtra->arr);
    free(_lpExtra);

    _lpExtra->arr = NULL;
    _lpExtra = NULL;

    return SUCCESS;
}