#include "Bars.h"
#include<stdio.h>

key_t CALLBACK CreateBars(CNT _cnt)
{
    LPVECBARS lpVecBars = NULL;
    key_t vecBarKey;
    GraphResourceAlloc(VECBARS,treelink,sizeof(VECBARS),lpVecBars,vecBarKey,DestoryBars,NULL);
    // lpVecBars = (LPVECBARS) realloc(lpVecBars,sizeof(VECBARS));
    CHECK_ERROR(lpVecBars == NULL,"Not able to allocate the vector of bars","CreateBars()");

    lpVecBars->lpBar = (LPBAR) xcalloc(_cnt * sizeof(BAR));
    lpVecBars->hBrushes = (HBRUSH*) xcalloc(_cnt * sizeof(HBRUSH));
    lpVecBars->DisplayBuffer = CreateDCLL();
    lpVecBars->iSize = _cnt;

    return vecBarKey;
};

LPDISPLAYBARATTR CreateDisplayBarAttr(LONG left,LONG right)
{
    LPDISPLAYBARATTR BarAttribute = (LPDISPLAYBARATTR) xcalloc(sizeof(DISPLAYBARATTR));
    BarAttribute->rect.left = left;
    BarAttribute->rect.right = right;

    BarAttribute->link.next = NULL;
    BarAttribute->link.prev = NULL;

    return BarAttribute;
}


/*

        Used to Give the Inital Values of Bars in the form of rects
        so that all other parameters are set by this InitBars()
        
        This accepts the three paramters

        1. The Vector which contains the bars
        2. The array of RECTS that should be the bars of graph
        3. Size of that array
*/ 
status InitBars(key_t hVecBars,const PRECT InlpRect,const CHAR** lables,const LPCOLORREF InlpColor,size_t _totRect)
{
    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);

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

status UpdateHeight(key_t hVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra)
{
    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);


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

status UpdateWidth(key_t hVecBars,EditFlags options,unsigned long int value,const LPEXTRA InExtra)
{
    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);

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

status_t GetVectorBarAttributes(key_t hVecBars,GetFlags options,LPVECBARS outVecBars)
{
    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);


    if(options | BR_SIZE)
    {
        outVecBars->iSize = _lpVecBars->iSize;
    }

    return SUCCESS;

}

status DrawBars(HDC hdc,PAINTSTRUCT *ps,key_t hVecBars,scrollOption option,int vPos,LPSCROLLINFO hScrollInfo)
{

    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);

    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);
    RECT rcTmp;
    RECT rcOverlap;

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
        LPNODE p_run = NULL;
        p_run = _lpVecBars->DisplayBuffer->next;

        for(int i = hScrollInfo->nPos,j=0;
                j < hScrollInfo->nPage && i < _lpVecBars->iSize;
                i++,j++
            )
        {

            // CopyRect(&(_lpVecBars->DisplayRect[j]),&(_lpVecBars->lpBar[j]._box));

            // _lpVecBars->DisplayRect[j].top = _lpVecBars->lpBar[i]._box.top;

            // _lpVecBars->DisplayRect[j].bottom = _lpVecBars->lpBar[i]._box.bottom;

            LPDISPLAYBARATTR DisplayBar = CONTAINER_OF(p_run,DISPLAYBARATTR,link);
            DisplayBar->rect.top = _lpVecBars->lpBar[i]._box.top;
            DisplayBar->rect.bottom = _lpVecBars->lpBar[i]._box.bottom;

            p_run = p_run->next;
        }
    }

    if(option & BR_VSCROLL)
    {

        /*
        

        */

        LPNODE p_run = NULL;
        p_run = _lpVecBars->DisplayBuffer->next;

        for(int i=hScrollInfo->nPos,j=0;i<nSize;i++,j++)
        {
            LPDISPLAYBARATTR DisplayBar = NULL;
            DisplayBar = CONTAINER_OF(p_run,DISPLAYBARATTR,link);

            // _lpVecBars->DisplayRect[j].top = 
            //     _lpVecBars->DisplayRect[j].top + (RESY * vPos) < 0 ? 0 : _lpVecBars->DisplayRect[j].top + (RESY * vPos);
            
            DisplayBar->rect.top = DisplayBar->rect.top + (RESY * vPos) < 0 ? 0 : DisplayBar->rect.top + (RESY * vPos);

            if(IntersectRect(&rcOverlap,&(ps->rcPaint),&(DisplayBar->rect)))
            {
                Rectangle(hdc,DisplayBar->rect.left,DisplayBar->rect.top,DisplayBar->rect.right,DisplayBar->rect.bottom);
                FillRect(hdc,&(DisplayBar->rect),_lpVecBars->hBrushes[i]);
            }

            // if(IntersectRect(&rcOverlap,&(ps->rcPaint),&(_lpVecBars->DisplayRect[j])))
            // {
            //     Rectangle(hdc,_lpVecBars->DisplayRect[j].left,_lpVecBars->DisplayRect[j].top,_lpVecBars->DisplayRect[j].right,_lpVecBars->DisplayRect[j].bottom);
            //     FillRect(hdc,&(_lpVecBars->DisplayRect[j]),_lpVecBars->hBrushes[i]);
            // }

            int Xmid = (    DisplayBar->rect.left
                                            + 
                                DisplayBar->rect.right
                        ) / 2;
            SetTextAlign(hdc,TA_CENTER);
            TextOut(hdc,Xmid,_lpVecBars->lpBar[i]._box.bottom + LBL_OFFSET,_lpVecBars->lpBar[i].lable,strnlen_s(_lpVecBars->lpBar[i].lable,MAX_LABLE_LENGTH));

            p_run = p_run->next;
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

status SetDisplayRectBuffer(key_t hVecBars,int nSize)
{
    LPVECBARS _lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(hVecBars,&(treenode));
    _lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);

    int genesis = _lpVecBars->lpBar[0]._box.left;
    int width = _lpVecBars->lpBar[0]._box.right - _lpVecBars->lpBar[0]._box.left;

    int gap = _lpVecBars->lpBar[1]._box.left - _lpVecBars->lpBar[0]._box.right;
    
    for(size_t i = _lpVecBars->DisplayBuffer->nr_elements;i < nSize;i++)
    {
        LONG left = genesis + (i * (width + gap));
        LONG right = left + width;

        InsertEnd(_lpVecBars->DisplayBuffer,&(CreateDisplayBarAttr(left,right)->link));
    }

    
    // PRECT pRect = (PRECT) realloc(_lpVecBars->DisplayRect,nSize * sizeof(RECT));
    // _lpVecBars->DisplayRect = pRect;
    // ZeroMemory(pRect,nSize * sizeof(RECT));

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
    LPEXTRA lpExtra = (LPEXTRA) xcalloc(sizeof(EXTRA));

    CHECK_ERROR(lpExtra == NULL,"Not able to create Extra","SetExtra");

    lpExtra->arr = InArr;
    lpExtra->iSize = _length;
    lpExtra->start = _start;
    lpExtra->end = _end;

    return lpExtra;
}

status CALLBACK DestoryBars(bst_node_t* tree_node)
{

    LPVECBARS _lpVecBars = NULL;

    _lpVecBars = CONTAINER_OF(tree_node,VECBARS,treelink);

    if(_lpVecBars != NULL)
        return FAIL;

    DestoryDCLL(&(_lpVecBars->DisplayBuffer),DestroyDisplayBarAttr);

    for(int i=0;i<_lpVecBars->iSize;i++)
    {
        free(&(_lpVecBars->lpBar[i]._box));
        free(&(_lpVecBars->lpBar[i]._color));
        DeleteObject(_lpVecBars->hBrushes[i]);
    }

    free(_lpVecBars->lpBar);
    free(_lpVecBars->hBrushes);
    free(_lpVecBars);

    _lpVecBars->lpBar = NULL;
    _lpVecBars        = NULL;

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

status_t DestroyDisplayBarAttr(LPNODE rm_node)
{
    LPDISPLAYBARATTR lpDisplayBarArr = NULL;
    lpDisplayBarArr = CONTAINER_OF(rm_node,DISPLAYBARATTR,link);
    free(lpDisplayBarArr);

    return SUCCESS;
}