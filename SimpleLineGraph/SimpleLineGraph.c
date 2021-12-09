#include "SimpleLineGraph.h"



/*
        Adding more paramters so that rest of Graph info is 
        privided by the caller

*/

LPSIMPLELINEGRAPH glpSimpleGraph = NULL;

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int* ppPoints,int m,int scale,LPCOLORREF pColor,CHAR* xTitle,CHAR** labels)
{
    // Line Graph Specific Initializations

    (*OutSimpleLineGraph) = (LPSIMPLELINEGRAPH) xcalloc(sizeof(SIMPLELINEGRAPH));

    (*OutSimpleLineGraph)->Lines = (LPLINE) xcalloc(sizeof(LPLINE));
    (*OutSimpleLineGraph)->nSize = 1;
    (*OutSimpleLineGraph)->pointGap = 96;
    (*OutSimpleLineGraph)->lpCoOrdinateAxis = CreateCoOrdinateAxis();
    (*OutSimpleLineGraph)->lpScaleLine = CreateScaleLines();
    (*OutSimpleLineGraph)->Lines[0].DisplayLineBuffer = CreateDCLL();
    (*OutSimpleLineGraph)->genesisPoint = DEFAULT_GENESIS;

    // Line Specific Initializations
    (*OutSimpleLineGraph)->Lines[0].points = (LPPOINT) xcalloc(m * sizeof(POINT));
    (*OutSimpleLineGraph)->Lines[0].height = (unit*) xcalloc(m * sizeof(unit));
    (*OutSimpleLineGraph)->Lines[0].labels = labels;

    (*OutSimpleLineGraph)->Lines->nSize = m;
    
    // -------------- Find the Max Height of a Point And X CoOrdinate -----------------
    
    HDC hdc = GetDC(hWnd);
    int ResY = GetDeviceCaps(hdc,LOGPIXELSY);

    SetYCoOrdinates((*OutSimpleLineGraph),ppPoints,m,ResY,scale);

    ReleaseDC(NULL,hdc);
    hdc = NULL;

    // ------------------------------------------------------------------

    glpSimpleGraph = (*OutSimpleLineGraph);

    COORDINATEAXIS tmpCoOrdinateAxis;

    tmpCoOrdinateAxis.scale = scale;
    tmpCoOrdinateAxis.TitleX = xTitle;

    SetCoOrdinateParamters((*OutSimpleLineGraph)->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_SCALE | AX_TITLE_X);

    LINE tmpLine;

    hdc = GetDC(hWnd);

    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);
    
    ReleaseDC(hWnd,hdc);

    SendMessage(hWnd,LINE_GRAPH_SAVE,0,(LONG_PTR)(*OutSimpleLineGraph));

    MSG Msg;
    
    ShowWindow(hWnd,SW_NORMAL);
    UpdateWindow(hWnd);

    while(GetMessage(&Msg,(HWND)NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;

}

status SetYCoOrdinates(LPSIMPLELINEGRAPH lpSimpleLineGraph,int* ppPoints,size_t length,int ResY,int scale)
{
    

    unit maxHeight = 0;

    for(int i=0;i<length;i++)
    {
        unit heightMulScale = ( ppPoints[i] / scale) * ResY;
        lpSimpleLineGraph->Lines[0].points[i].y = heightMulScale;
        lpSimpleLineGraph->Lines[0].height[i] = heightMulScale;

        if(maxHeight <= heightMulScale)
            maxHeight = heightMulScale;
    }
    
    lpSimpleLineGraph->Lines[0].maxHeight = maxHeight;

    return SUCCESS;
}

status RelocateYCoordinateLines(LPSIMPLELINEGRAPH lpSimpleLineGraph)
{
    POINT Origin;
    GetOrigin(lpSimpleLineGraph->lpCoOrdinateAxis,&Origin);

    lpSimpleLineGraph->Lines[0].points[0].x = Origin.x + lpSimpleLineGraph->genesisPoint;

    lpSimpleLineGraph->Lines[0].points[0].y = Origin.y - lpSimpleLineGraph->Lines[0].height[0];

    for(int i=1;i<lpSimpleLineGraph->Lines[0].nSize;i++)
        lpSimpleLineGraph->Lines[0].points[i].y = Origin.y - lpSimpleLineGraph->Lines[0].height[i];

    #define AP(a,n,d) (a + (n * d))

    int length = lpSimpleLineGraph->Lines[0].nSize;

    int a = lpSimpleLineGraph->genesisPoint;
    int d = lpSimpleLineGraph->pointGap;

    

    for(int i=0;i<length;i++)
        lpSimpleLineGraph->Lines[0].points[i].x = Origin.x + AP(a,i,d);


    return SUCCESS;
}

LRESULT CALLBACK WndSimpleLineGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

    static int CxClient,CyClient;
    static int actualNMax;
    static int nHscrollAmt;

    static CHAR buff[100];

    static RECT rcVerticleScroll;
    static RECT rcHorzontalScroll;

    static SCROLLINFO horScrollInfo;
    static SCROLLINFO vertScrollInfo;
    static SCALELINE tmpScaleLine;
    static COORDINATEAXIS tmpCoOrdinateAxis;
    static PRECT pRect;
    static LPSIMPLELINEGRAPH lpSimpleLineGraph;
    static int RESY;
    // @vscroll pos
    static int iVscroll;

    //@hscroll pos
    static int iHscroll;

    HDC hdc;
    PAINTSTRUCT ps;
    

    switch(uMsg)
    {

        case LINE_GRAPH_SAVE:
            lpSimpleLineGraph = (LPSIMPLELINEGRAPH)lParam;
        break;

        case WM_CREATE:

        // Axis Initilization

        tmpCoOrdinateAxis.Origin = (LPPOINT) xcalloc(sizeof(POINT));
        tmpCoOrdinateAxis.XAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));
        tmpCoOrdinateAxis.YAxisEnd = (LPPOINT) xcalloc(sizeof(POINT));

        tmpScaleLine.StartPoint = (LPPOINT) xcalloc(sizeof(POINT));
        tmpScaleLine.EndPoint = (LPPOINT) xcalloc(sizeof(POINT));

        hdc = GetDC(hWnd);
        RESY = GetDeviceCaps(hdc,LOGPIXELSY);
        ReleaseDC(hWnd,hdc);

        break;

        case WM_SIZE:

        CxClient = LOWORD(lParam);
        CyClient = HIWORD(lParam);
        
        // Set the the Axis as per no of bars present in the graph

        unsigned long int a = lpSimpleLineGraph->genesisPoint;
        unsigned long int d = lpSimpleLineGraph->pointGap;
        unsigned long int n = lpSimpleLineGraph->Lines[0].nSize;

        #ifdef AP
        #undef AP
        #endif
        #define AP(a,n,d) ((a + ((n-1)*d)))

        tmpCoOrdinateAxis.Origin->x = RESY;
        tmpCoOrdinateAxis.Origin->y = CyClient - RESY;

        tmpCoOrdinateAxis.XAxisEnd->x = tmpCoOrdinateAxis.Origin->x + AP(a,n,d);
        tmpCoOrdinateAxis.XAxisEnd->y = tmpCoOrdinateAxis.Origin->y;

        tmpCoOrdinateAxis.YAxisEnd->x = tmpCoOrdinateAxis.Origin->x;
        tmpCoOrdinateAxis.YAxisEnd->y = tmpCoOrdinateAxis.Origin->y - lpSimpleLineGraph->Lines[0].maxHeight;


        tmpScaleLine.StartPoint->x  =   tmpCoOrdinateAxis.Origin->x;
        tmpScaleLine.StartPoint->y  =   tmpCoOrdinateAxis.Origin->y;

        tmpScaleLine.EndPoint->x    =   tmpCoOrdinateAxis.XAxisEnd->x;
        tmpScaleLine.EndPoint->y    =   tmpCoOrdinateAxis.YAxisEnd->y;

        GetScale(lpSimpleLineGraph->lpCoOrdinateAxis,&tmpCoOrdinateAxis);

        tmpScaleLine.Space          =   tmpCoOrdinateAxis.scale;
  

        hdc = GetDC(hWnd);

        // Get the Y Resolution

        RESY = GetDeviceCaps(hdc,LOGPIXELSY);

        ReleaseDC(hWnd,hdc);

        // Set the CoOrdinate Axis

        SetCoOrdinateParamters(lpSimpleLineGraph->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_INIT_X_AXIS | AX_INIT_Y_AXIS);

        // Relocate the Lines
        RelocateYCoordinateLines(lpSimpleLineGraph);

        // Set the scale Lines
        SetScaleLines( lpSimpleLineGraph->lpScaleLine,
                        SCLE_START_PT | SCLE_END_PT | SCLE_SPACE,
                        &tmpScaleLine
                    );


            /*
                @Vertical Scroll
            */

        vertScrollInfo.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
        GetScrollInfo(hWnd,SB_VERT,&vertScrollInfo);

        int oldActualMax = vertScrollInfo.nPage == 0 ? 0 : vertScrollInfo.nMax - vertScrollInfo.nPage + 1;
        int oldnPos = vertScrollInfo.nPos; 

        vertScrollInfo.fMask    = SIF_RANGE | SIF_PAGE | SIF_POS;
        vertScrollInfo.cbSize   = sizeof(SCROLLBARINFO);
        vertScrollInfo.nMin     = 0;
        vertScrollInfo.nMax     = (lpSimpleLineGraph->Lines[0].maxHeight/ RESY);
        vertScrollInfo.nPage    = (tmpCoOrdinateAxis.Origin->y) / RESY;

        actualNMax = vertScrollInfo.nMax - vertScrollInfo.nPage + 1;

        /*
            if nPos == 0
                and     oldActualMax == 0 then set scrollbar to bottom
                        oldActualMax != 0 then set scroll to top;
            else

                maximum of 0 and actualNMax - (oldActualMax - vertScrollInfo.nPos)
        */

        if(vertScrollInfo.nPos == 0)
            vertScrollInfo.nPos = oldActualMax == 0 ? actualNMax : 0;
        else
            vertScrollInfo.nPos = max(0,actualNMax - (oldActualMax - vertScrollInfo.nPos));

        SetScrollInfo(hWnd,SB_VERT,&vertScrollInfo,TRUE);

        /*
                @Horizontal Scroll
        */
        horScrollInfo.cbSize = sizeof(SCROLLINFO);

        horScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        GetScrollInfo(hWnd,SB_HORZ,&horScrollInfo);
        int oldActualHorzMax = horScrollInfo.nPage == 0 ? -1 : horScrollInfo.nMax - horScrollInfo.nPage + 1;
        int oldPos = horScrollInfo.nPos;

        horScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        horScrollInfo.nMin = 0;
        horScrollInfo.nMax = (tmpCoOrdinateAxis.XAxisEnd->x - tmpCoOrdinateAxis.Origin->x - a) / d;
        horScrollInfo.nPage = (CxClient - tmpCoOrdinateAxis.Origin->x - a) 
                                        / d;
                                        
        if(oldPos == oldActualHorzMax)
            horScrollInfo.nPos = horScrollInfo.nMax - horScrollInfo.nPage + 1;
        else 
            horScrollInfo.nPos = min(oldPos,horScrollInfo.nMax - horScrollInfo.nPage + 1);
        SetScrollInfo(hWnd,SB_HORZ,&horScrollInfo,TRUE); 

        SetDisplayLineBuffer(lpSimpleLineGraph,horScrollInfo.nPage);

        // Set the ScrollAmoubt for Horizontal scroll
        nHscrollAmt = d;  

        // Set the Scroll Area

        SetRect(&rcVerticleScroll,0,0,CxClient,tmpCoOrdinateAxis.XAxisEnd->y);

        SetRect(&rcHorzontalScroll,tmpCoOrdinateAxis.Origin->x + a,0,tmpCoOrdinateAxis.Origin->x + AP(a,horScrollInfo.nPage + 1,d),tmpCoOrdinateAxis.Origin->y + 25);

        

        break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd,&ps);

            DrawCoOrdinateAxis( hdc,&ps,
                                lpSimpleLineGraph->lpCoOrdinateAxis,
                                AX_HSCROLL | AX_VSCROLL,
                                actualNMax - vertScrollInfo.nPos,
                                nHscrollAmt * horScrollInfo.nPos
                            );

            DrawScaleLines(     hdc,&ps,
                                lpSimpleLineGraph->lpScaleLine,
                                SCLE_VSCROLL | SCLE_HSCROLL,
                                actualNMax - vertScrollInfo.nPos
                          );
            DrawLines(
                            hdc,
                            &ps,
                            lpSimpleLineGraph->Lines,
                            LNE_VSCROLL | LNE_HSCROLL,
                            actualNMax - vertScrollInfo.nPos,
                            &horScrollInfo,
                            RGB(255,0,0),
                            tmpCoOrdinateAxis.Origin->y
            );

            // Rectangle(hdc,rcHorzontalScroll.left,rcHorzontalScroll.top,rcHorzontalScroll.right,rcHorzontalScroll.bottom);

            EndPaint(hWnd,&ps);
            hdc = NULL;
        break;

        case WM_DESTROY:
        PostQuitMessage(EXIT_FAILURE);
        break;

        case WM_VSCROLL:

        vertScrollInfo.cbSize = sizeof(vertScrollInfo);
        vertScrollInfo.fMask = SIF_ALL;
        GetScrollInfo(hWnd,SB_VERT,&vertScrollInfo);

        iVscroll = vertScrollInfo.nPos;

        switch(LOWORD(wParam))
        {
            case SB_LINEDOWN:
            vertScrollInfo.nPos += 1;
            break;

            case SB_LINEUP:
            vertScrollInfo.nPos -= 1;
            break;

            case SB_PAGEUP:
            vertScrollInfo.nPos -= vertScrollInfo.nPage;
            break;

            case SB_PAGEDOWN:
            vertScrollInfo.nPos += vertScrollInfo.nPage;
            break;

            case SB_THUMBTRACK:
            vertScrollInfo.nPos = vertScrollInfo.nTrackPos;
            break;
        }

        vertScrollInfo.fMask = SIF_POS;
        SetScrollInfo(hWnd,SB_VERT,&vertScrollInfo,TRUE);
        GetScrollInfo(hWnd,SB_VERT,&vertScrollInfo);

        

        if(vertScrollInfo.nPos != iVscroll)
        {
            ScrollWindow(hWnd,0, RESY * (iVscroll - vertScrollInfo.nPos),&rcVerticleScroll,&rcVerticleScroll);
            UpdateWindow(hWnd);
        }
        break;

        case WM_HSCROLL:

            horScrollInfo.cbSize = sizeof(SCROLLINFO);
            horScrollInfo.fMask = SIF_ALL;
            GetScrollInfo(hWnd,SB_HORZ,&horScrollInfo);

            iHscroll = horScrollInfo.nPos;

            switch(LOWORD(wParam))
            {

                case SB_LINELEFT:
                    horScrollInfo.nPos -= 1;
                break;

                case SB_LINERIGHT:
                    horScrollInfo.nPos += 1;
                break;

                case SB_PAGELEFT:
                    horScrollInfo.nPos -= horScrollInfo.nPage;
                break;

                case SB_PAGERIGHT:
                    horScrollInfo.nPos += horScrollInfo.nPage;
                break;

                case SB_THUMBTRACK:
                    horScrollInfo.nPos = horScrollInfo.nTrackPos;
                break;
            }

            horScrollInfo.fMask = SIF_POS;
            SetScrollInfo(hWnd,SB_HORZ,&horScrollInfo,TRUE);
            GetScrollInfo(hWnd,SB_HORZ,&horScrollInfo);

            if(iHscroll != horScrollInfo.nPos)
            {
                ScrollWindow(hWnd,nHscrollAmt * (iHscroll - horScrollInfo.nPos),0,&rcHorzontalScroll,&rcHorzontalScroll);
                UpdateWindow(hWnd);
            }


        break;

    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

status DrawLines(HDC hdc,PAINTSTRUCT* ps,LPLINE lpLine,scrollOption option,int vPos,LPSCROLLINFO hScrollInfo,COLORREF color,LONG axisY)
{
    int PPI = GetDeviceCaps(hdc,LOGPIXELSY);
    int nSize = (lpLine->nSize <= (hScrollInfo->nPos + hScrollInfo->nPage)
                                    ?
                            lpLine->nSize
                                    :
                  hScrollInfo->nPos + hScrollInfo->nPage
                );

    HPEN hPrevPen = NULL;
    HPEN hPen = CreatePen(PS_SOLID,3,color);
   
    if(option & LNE_HSCROLL)
    {
        LPNODE p_run = NULL;
        p_run = lpLine->DisplayLineBuffer->next;

        SetTextAlign(hdc,TA_CENTER);

        for(int i = hScrollInfo->nPos,j=0;
                j < hScrollInfo->nPage;
                i++,j++
            )
        {
            LPBUFFERPOINTS PointBuffer = NULL;
            PointBuffer = CONTAINER_OF(p_run,BUFFERPOINTS,link);

            PointBuffer->point.y = lpLine->points[i].y;
            PointBuffer->label = lpLine->labels[i];

            TextOut(hdc,PointBuffer->point.x,axisY + 10,PointBuffer->label,strnlen(PointBuffer->label,100));

            p_run = p_run->next;
        }
    }

    if(option & LNE_VSCROLL)
    {
        LPNODE p_run = NULL;
        p_run = lpLine->DisplayLineBuffer->next;

        for(int i=hScrollInfo->nPos;i<hScrollInfo->nPage;i++)
        {
            LPBUFFERPOINTS PointBuffer = NULL;
            PointBuffer = CONTAINER_OF(p_run,BUFFERPOINTS,link);

            PointBuffer->point.y = lpLine->points[i].y + (PPI * vPos);

            p_run = p_run->next;
        }
    }

    LPNODE p_run = NULL;
    GetBegin(lpLine->DisplayLineBuffer,&p_run);

    LPBUFFERPOINTS StartBufferPoint = CONTAINER_OF(p_run,BUFFERPOINTS,link);
    LPBUFFERPOINTS BufferPoints = NULL;

    hPrevPen = SelectObject(hdc,hPen);

    MoveToEx(hdc,StartBufferPoint->point.x,StartBufferPoint->point.y,NULL);
    p_run = p_run->next;

    for(int i=1;i<hScrollInfo->nPage;i++)
    {
        BufferPoints = CONTAINER_OF(p_run,BUFFERPOINTS,link);
        LineTo(hdc,BufferPoints->point.x,BufferPoints->point.y);

        p_run = p_run->next;
    }

    SelectObject(hdc,hPrevPen);

    return SUCCESS;
}

LPBUFFERPOINTS CreateBufferPoint(LONG x)
{
    LPBUFFERPOINTS BufferPoint = xmalloc(sizeof(BUFFERPOINTS));
    BufferPoint->point.x = x;
    BufferPoint->point.y = 0;

    return BufferPoint;
}

status SetDisplayLineBuffer(LPSIMPLELINEGRAPH lpSimpleLineGraph,size_t nSize)
{
    // LPPOINT ptr = (LPPOINT) realloc(lpSimpleLineGraph->Lines[0].lpDisplayBuffer,size * sizeof(POINT));
    // // ZeroMemory(ptr,size * sizeof(POINT));

    // lpSimpleLineGraph->Lines[0].lpDisplayBuffer = ptr;
    POINT Origin;
    GetOrigin(lpSimpleLineGraph->lpCoOrdinateAxis,&Origin);
    int genesis = Origin.x + lpSimpleLineGraph->genesisPoint;

    int gap = lpSimpleLineGraph->pointGap;
    
    for(    size_t i = lpSimpleLineGraph->Lines[0].DisplayLineBuffer->nr_elements;
            i < nSize;
            i++
        )
    {
        LONG x = genesis + (i *  gap);

        InsertEnd(lpSimpleLineGraph->Lines[0].DisplayLineBuffer,&(CreateBufferPoint(x)->link));

    }

    return SUCCESS;
}

status DestroyLine(LPLINE lpLine)
{
    if(lpLine == NULL)
        return SUCCESS;
    
    DestoryDCLL(&(lpLine->DisplayLineBuffer),DestoryBufferPoint);

    free(lpLine->height);
    free(lpLine->lpDisplayBuffer);
    free(lpLine->points);

    return SUCCESS;
}

status_t DestoryBufferPoint(LPNODE rm_node)
{
    LPBUFFERPOINTS rmBuffer = CONTAINER_OF(rm_node,BUFFERPOINTS,link);
    free(rmBuffer);

    return SUCCESS;
}

status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph)
{
    if(lpSimpleLineGraph == NULL)
        return SUCCESS;

    DeAllocate(lpSimpleLineGraph->lpCoOrdinateAxis);
    DeAllocate(lpSimpleLineGraph->lpScaleLine);
    DestroyLine(&(lpSimpleLineGraph->Lines[0]));

    free(lpSimpleLineGraph->Lines);

    lpSimpleLineGraph->Lines = NULL;
    lpSimpleLineGraph = NULL;

    free(lpSimpleLineGraph);
    return SUCCESS;
}