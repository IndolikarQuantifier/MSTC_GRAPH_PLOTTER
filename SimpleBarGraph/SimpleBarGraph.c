#include "SimpleBarGraph.h"
#include<math.h>

LRESULT CALLBACK WndSimpleBarGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

// LPSIMPLEBARGRAPH glpSimpleBarGraph = NULL;


/*
        Adding more paramters so that rest of Graph info is 
        privided by the caller

*/

int CreateBarGraph(HWND hWnd,LPSIMPLEBARGRAPH* lpSimpleBarGraph,unit* Heights,LPCOLORREF lpColor,unit scale,size_t iSize,const LPSTR xTitle,const LPSTR yTitle,const CHAR** lables)
{
    
    *lpSimpleBarGraph = (LPSIMPLEBARGRAPH) xcalloc(sizeof(SIMPLEBARGRAPH));
    CHECK_ERROR((*lpSimpleBarGraph) == NULL,
            "Not able to allocate memory for SimpleBarGraph",
            "CreatebarGraph()"
        );

    (*lpSimpleBarGraph)->gap = GAP;
    (*lpSimpleBarGraph)->lpCoOrdinateAxis = CreateCoOrdinateAxis();
    (*lpSimpleBarGraph)->lpVecBars = CreateBars(iSize);
    (*lpSimpleBarGraph)->lpScaleLine = CreateScaleLines();

    (*lpSimpleBarGraph)->barWdth = BARWDTH;
    (*lpSimpleBarGraph)->genesisBar = GENESIS_BAR;
    (*lpSimpleBarGraph)->maxHeight = 0;

    // glpSimpleBarGraph = *lpSimpleBarGraph;


    COORDINATEAXIS tmpCoOrdinateAxis;
    tmpCoOrdinateAxis.scale = scale;
    tmpCoOrdinateAxis.TitleX = xTitle;
    tmpCoOrdinateAxis.TitleY = yTitle;

    SetCoOrdinateParamters((*lpSimpleBarGraph)->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_SCALE | AX_TITLE_X | AX_TITLE_Y);

    // MessageBox((HWND)NULL,"TitleY",xTitle,MB_ICONERROR);

    CreateDataBars((*lpSimpleBarGraph),Heights,lpColor,lables,iSize);

    SendMessage(hWnd,BRGRPH_SAVE,0,(LONG_PTR)(*lpSimpleBarGraph));

    MSG Msg;
    
    ShowWindow(hWnd,SW_NORMAL);
    UpdateWindow(hWnd);
    
    while(GetMessage(&Msg,(HWND)NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    DestroySimpleBarGraph(*(lpSimpleBarGraph));
    *lpSimpleBarGraph = NULL;
    // free(data);
    return Msg.wParam;

}


/*
        Avoid the global Pointer of Simple Bar graph
*/

status CreateDataBars(LPSIMPLEBARGRAPH lpSimpleBarGraph,unit* barData,LPCOLORREF lpColor,const CHAR** lables,size_t iSize)
{
    PRECT pRect = (PRECT) xcalloc(iSize * sizeof(RECT));
    CHECK_ERROR(pRect == NULL,"Not able to allocate the memory for rect","CreateDataBars()");
    lpSimpleBarGraph->maxHeight = 0;

    HDC hdc = GetDC(NULL);

    int RESOLUTOIN_Y = GetDeviceCaps(hdc,LOGPIXELSY);

    // static CHAR buff[1000];
    // wsprintf(buff,"Resolution : %d",RESOLUTOIN_Y);
    // MessageBox((HWND)NULL,"Res",buff,MB_ICONERROR);
    COORDINATEAXIS tmpCoOrdinateAxis;
    GetScale(lpSimpleBarGraph->lpCoOrdinateAxis,&tmpCoOrdinateAxis);
    for(int i=0;i<iSize;i++)
    {
        double height = (((barData[i] / tmpCoOrdinateAxis.scale) * 1.0) * RESOLUTOIN_Y);
        // wsprintf(buff,"barData : %d scale :%d Res : %d height : %d",barData[i],lpSimpleBarGraph->lpCoOrdinateAxis->scale,RESOLUTOIN_Y,height);
        // MessageBox((HWND)NULL,buff,"height",MB_ICONERROR);
        SetRect((pRect + i),0,0,96,height);
        lpSimpleBarGraph->maxHeight = max(lpSimpleBarGraph->maxHeight,height);
    }
    
    InitBars(lpSimpleBarGraph->lpVecBars,pRect,lables,lpColor,iSize);
    ReleaseDC(NULL,hdc);
    hdc = NULL;

    return SUCCESS;
}

status ReLocateBars(LPSIMPLEBARGRAPH lpSimpleBarGraph)
{
    LPVECBARS lpVecBars = NULL;
    bst_node_t* treenode = NULL;

    GetGraphResource(lpSimpleBarGraph->lpVecBars,&(treenode));
    lpVecBars = CONTAINER_OF(treenode,VECBARS,treelink);

    POINT origin;
    GetOrigin(lpSimpleBarGraph->lpCoOrdinateAxis,&origin);

    

    lpVecBars->lpBar[0]._box.left  = origin.x + lpSimpleBarGraph->genesisBar;
    lpVecBars->lpBar[0]._box.right = lpVecBars->lpBar[0]._box.left + lpSimpleBarGraph->barWdth;

    lpVecBars->lpBar[0]._box.bottom = origin.y;
    lpVecBars->lpBar[0]._box.top = lpVecBars->lpBar[0]._box.bottom - lpVecBars->lpBar[0]._height;

    for(int i=1;i<lpVecBars->iSize;i++)
    {
        lpVecBars->lpBar[i]._box.left = lpVecBars->lpBar[i-1]._box.right + lpSimpleBarGraph->gap;
        lpVecBars->lpBar[i]._box.right = lpVecBars->lpBar[i]._box.left + lpSimpleBarGraph->barWdth;

        lpVecBars->lpBar[i]._box.bottom = origin.y;
        lpVecBars->lpBar[i]._box.top = lpVecBars->lpBar[i]._box.bottom - lpVecBars->lpBar[i]._height;

    }

    return SUCCESS;
}

void DrawSimpleBarGraph(HDC hdc,LPPAINTSTRUCT ps,LPSIMPLEBARGRAPH lpSimpleBarGraph)
{

}

void DestroySimpleBarGraph(LPSIMPLEBARGRAPH lpSimpleBarGraph)
{
    if(lpSimpleBarGraph != NULL)
    {
        // DestoryCoOrdinateAxis(lpSimpleBarGraph->lpCoOrdinateAxis);
        
        // DestoryBars(lpSimpleBarGraph->lpVecBars);
        // DestoryScaleLines(lpSimpleBarGraph->lpScaleLine);
        free(lpSimpleBarGraph);

        lpSimpleBarGraph->lpVecBars         = -1;
        lpSimpleBarGraph->lpCoOrdinateAxis  = -1;
        lpSimpleBarGraph                    = NULL;
    }
    
}

/*
    TODO :
    Need to add a Rectangle ViewPort So that we can add a Info Box there
*/

LRESULT CALLBACK WndSimpleBarGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
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
    static VECBARS tmpVecBars;
    static PRECT pRect;
    static LPSIMPLEBARGRAPH lpSimpleBarGraph;
    static int RESY;
    // @vscroll pos
    static int iVscroll;

    //@hscroll pos
    static int iHscroll;

    HDC hdc;
    PAINTSTRUCT ps;
    

    switch(uMsg)
    {

        case BRGRPH_SAVE :
            lpSimpleBarGraph = (LPSIMPLEBARGRAPH)lParam;
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
        GetVectorBarAttributes(lpSimpleBarGraph->lpVecBars,BR_SIZE,&tmpVecBars);

        unsigned long int a = lpSimpleBarGraph->genesisBar;
        unsigned long int d = lpSimpleBarGraph->gap + lpSimpleBarGraph->barWdth;
        unsigned long int n = tmpVecBars.iSize;
        
        #define AP(a,n,d) ((a + ((n-1)*d)))

        tmpCoOrdinateAxis.Origin->x = RESY;
        tmpCoOrdinateAxis.Origin->y = CyClient - RESY;

        tmpCoOrdinateAxis.XAxisEnd->x = tmpCoOrdinateAxis.Origin->x + AP(a,n,d) + lpSimpleBarGraph->barWdth + OFFSET;
        tmpCoOrdinateAxis.XAxisEnd->y = tmpCoOrdinateAxis.Origin->y;

        tmpCoOrdinateAxis.YAxisEnd->x = tmpCoOrdinateAxis.Origin->x;
        tmpCoOrdinateAxis.YAxisEnd->y = tmpCoOrdinateAxis.Origin->y - lpSimpleBarGraph->maxHeight - OFFSET;
        GetScale(lpSimpleBarGraph->lpCoOrdinateAxis,&tmpCoOrdinateAxis);
        // Set the Scale Lines Parameters

        tmpScaleLine.StartPoint->x  =   tmpCoOrdinateAxis.Origin->x;
        tmpScaleLine.StartPoint->y  =   tmpCoOrdinateAxis.Origin->y;

        tmpScaleLine.EndPoint->x    =   tmpCoOrdinateAxis.XAxisEnd->x;
        tmpScaleLine.EndPoint->y    =   tmpCoOrdinateAxis.YAxisEnd->y;

        tmpScaleLine.Space          =   tmpCoOrdinateAxis.scale;
  

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
            
            DrawBars(   hdc,&ps,
                        lpSimpleBarGraph->lpVecBars,
                        BR_VSCROLL | BR_HSCROLL,
                        actualNMax - vertScrollInfo.nPos,
                        &horScrollInfo
                    );

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