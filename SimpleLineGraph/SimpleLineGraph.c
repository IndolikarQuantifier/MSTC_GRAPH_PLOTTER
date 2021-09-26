#include "SimpleLineGraph.h"

LRESULT CALLBACK WndSimpleLineGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

/*
        Adding more paramters so that rest of Graph info is 
        privided by the caller

*/

LPSIMPLELINEGRAPH glpSimpleGraph = NULL;

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int** ppPoints,int m,int* nPSize,int scale,LPCOLORREF pColor)
{
    
    (*OutSimpleLineGraph) = (LPSIMPLELINEGRAPH) xcalloc(sizeof(SIMPLELINEGRAPH));

    (*OutSimpleLineGraph)->Lines = (LPLINE*) xcalloc(m * sizeof(LPLINE));
    (*OutSimpleLineGraph)->nSize = m;
    (*OutSimpleLineGraph)->pointGap = 10;
    (*OutSimpleLineGraph)->lpCoOrdinateAxis = CreateCoOrdinateAxis();
    (*OutSimpleLineGraph)->genesisPoint = DEFAULT_GENESIS;

    glpSimpleGraph = (*OutSimpleLineGraph);

    COORDINATEAXIS tmpCoOrdinateAxis;

    tmpCoOrdinateAxis.scale = scale;

    SetCoOrdinateParamters((*OutSimpleLineGraph)->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_SCALE);

    LINE tmpLine;

    HDC hdc = GetDC(hWnd);

    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);

    for(int i=0;i<m;i++)
    {
        tmpLine.pPoints = (LPPOINT) xcalloc(nPSize[i],sizeof(POINT));
        
        for(int j=0;j<nPSize[i];j++)
        {
            ppPoints[i][j] = (ppPoints[i][j] / scale * RESY);
            tmpLine.pPoints[j].y = ppPoints[i][j];
        }
        tmpLine.nSize = nPSize[i];
        tmpLine.colLine = pColor[i];
        SetLine((*OutSimpleLineGraph)->Lines + i,&tmpLine,LNE_POINTS | LNE_COLOR);
    }
    
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

status RelocateLines(LPSIMPLELINEGRAPH lpSimpleLineGraph,int** ppPoints,int m,int* nPSize)
{
    // for(int i=0;i<m;i++)
    // {
    //     for(int j=0;j<nPSize[i];j++)
    //     {

    //     }
    // }
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
        unsigned long int n = lpSimpleLineGraph->maxPoints;
        
        #define AP(a,n,d) ((a + ((n-1)*d)))

        tmpCoOrdinateAxis.Origin->x = RESY;
        tmpCoOrdinateAxis.Origin->y = CyClient - RESY;

        tmpCoOrdinateAxis.XAxisEnd->x = tmpCoOrdinateAxis.Origin->x + AP(a,n,d) + lpSimpleLineGraph->barWdth + OFFSET;
        tmpCoOrdinateAxis.XAxisEnd->y = tmpCoOrdinateAxis.Origin->y;

        tmpCoOrdinateAxis.YAxisEnd->x = tmpCoOrdinateAxis.Origin->x;
        tmpCoOrdinateAxis.YAxisEnd->y = tmpCoOrdinateAxis.Origin->y - lpSimpleLineGraph->maxHeight - OFFSET;

        // Set the Scale Lines Parameters

        tmpScaleLine.StartPoint->x  =   tmpCoOrdinateAxis.Origin->x;
        tmpScaleLine.StartPoint->y  =   tmpCoOrdinateAxis.Origin->y;

        tmpScaleLine.EndPoint->x    =   tmpCoOrdinateAxis.XAxisEnd->x;
        tmpScaleLine.EndPoint->y    =   tmpCoOrdinateAxis.YAxisEnd->y;

        tmpScaleLine.Space          =   lpSimpleBarGraph->lpCoOrdinateAxis->scale;
  

        hdc = GetDC(hWnd);

        // Get the Y Resolution

        RESY = GetDeviceCaps(hdc,LOGPIXELSY);

        ReleaseDC(hWnd,hdc);

        // Set the CoOrdinate Axis

        SetCoOrdinateParamters(lpSimpleBarGraph->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_INIT_X_AXIS | AX_INIT_Y_AXIS);

        // Relocate the Bars
        ReLocateBars(lpSimpleBarGraph);

        // Set the scale Lines
        SetScaleLines( lpSimpleBarGraph->lpScaleLine,
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
        vertScrollInfo.nMax     = (lpSimpleBarGraph->maxHeight/ RESY);
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
        horScrollInfo.nMax = (tmpCoOrdinateAxis.XAxisEnd->x - tmpCoOrdinateAxis.Origin->x - a) 
                                            / d;
        horScrollInfo.nPage = (CxClient - tmpCoOrdinateAxis.Origin->x - a) 
                                        / d;
                                        
        if(oldPos == oldActualHorzMax)
            horScrollInfo.nPos = horScrollInfo.nMax - horScrollInfo.nPage + 1;
        else 
            horScrollInfo.nPos = min(oldPos,horScrollInfo.nMax - horScrollInfo.nPage + 1);
        SetScrollInfo(hWnd,SB_HORZ,&horScrollInfo,TRUE); 

        SetDisplayRectBuffer(lpSimpleBarGraph->lpVecBars,horScrollInfo.nPage);

        // Set the ScrollAmoubt for Horizontal scroll
        nHscrollAmt = d;  

        // Set the Scroll Area

        SetRect(&rcVerticleScroll,0,0,CxClient,tmpCoOrdinateAxis.XAxisEnd->y);

        SetRect(&rcHorzontalScroll,tmpCoOrdinateAxis.Origin->x + a,0,tmpCoOrdinateAxis.Origin->x + AP(a,horScrollInfo.nPage + 1,d),tmpCoOrdinateAxis.Origin->y + 25);

        break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd,&ps);

            DrawCoOrdinateAxis( hdc,&ps,
                                lpSimpleBarGraph->lpCoOrdinateAxis,
                                AX_VSCROLL | AX_HSCROLL,
                                actualNMax - vertScrollInfo.nPos,
                                nHscrollAmt * horScrollInfo.nPos
                            );

            DrawScaleLines(     hdc,&ps,
                                lpSimpleBarGraph->lpScaleLine,
                                SCLE_VSCROLL | SCLE_HSCROLL,
                                actualNMax - vertScrollInfo.nPos
                          );
            
            // DrawBars(   hdc,&ps,
            //             lpSimpleBarGraph->lpVecBars,
            //             BR_VSCROLL | BR_HSCROLL,
            //             actualNMax - vertScrollInfo.nPos,
            //             &horScrollInfo
            //         );

            // Rectangle(hdc,rcHorzontalScroll.left,rcHorzontalScroll.top,rcHorzontalScroll.right,rcHorzontalScroll.bottom);

            EndPaint(hWnd,&ps);
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
            // case SB_TOP:
            // vertScrollInfo.nPos = vertScrollInfo.nMin;
            // break;

            // case SB_BOTTOM:
            // vertScrollInfo.nPos = vertScrollInfo.nMax;
            // break;

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
                case SB_LEFT:
                    horScrollInfo.nPos = horScrollInfo.nMin;
                break;

                case SB_RIGHT:
                    horScrollInfo.nPos = horScrollInfo.nMax;
                break;

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

status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph)
{
    if(lpSimpleLineGraph == NULL)
        return FAIL;
    
    for(int i=0;i < lpSimpleLineGraph->nSize;i++)
    {
        DestoryLine(&(lpSimpleLineGraph->Lines[i]));
    }

    lpSimpleLineGraph->Lines = NULL;
    lpSimpleLineGraph = NULL;

    return SUCCESS;
}