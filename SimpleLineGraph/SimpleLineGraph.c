#include "SimpleLineGraph.h"

LRESULT CALLBACK WndSimpleLineGraphProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

/*
        Adding more paramters so that rest of Graph info is 
        privided by the caller

*/

LPSIMPLELINEGRAPH glpSimpleGraph = NULL;

HWND Init(HINSTANCE hInstance,HWND hPrentWnd)
{
    static TCHAR lpszGraphClsName[] = TEXT("BarGraph");
    static TCHAR lpszGraphCaption[] = TEXT("SimpleBar Graph !!!");

    WNDCLASSEX wndGraphEx;
    

    HWND hWndGraph;

    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;
    
    hBrush = (HBRUSH) GetStockObject(WHITE_BRUSH);
    CHECK_ERROR(hBrush == NULL,"Not able to allocate the memory to Brush","GetStockObject()");

    hCursor = LoadCursor((HINSTANCE)NULL,IDC_ARROW);
    CHECK_ERROR(hCursor == NULL,"Not able to allocate the memory to Cursor","LoadCursor()");

    hIcon = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    CHECK_ERROR(hIcon == NULL,"Not able to allocate the memory to Icon","LoadIcon()");

    hIconSm = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    CHECK_ERROR(hIconSm == NULL,"Not able to allocate the memory to IconSm","LoadIcon()");

    wndGraphEx.cbSize = sizeof(wndGraphEx);
    wndGraphEx.cbClsExtra = 0;
    wndGraphEx.cbWndExtra = 0;

    wndGraphEx.lpfnWndProc = WndSimpleLineGraphProc;
    wndGraphEx.lpszClassName = lpszGraphClsName;
    wndGraphEx.lpszMenuName = NULL;

    wndGraphEx.hbrBackground = hBrush;
    wndGraphEx.hIcon = hIcon;
    wndGraphEx.hIconSm = hIconSm;
    wndGraphEx.hCursor = hCursor;

    wndGraphEx.style = CS_HREDRAW | CS_VREDRAW;
    wndGraphEx.hInstance = hInstance;

    if(!RegisterClassEx(&wndGraphEx))
    {
        MessageBox((HWND)NULL,TEXT("Not able to Register the event"),TEXT("RegisterClassEx()"),MB_ICONERROR);
        ExitProcess(EXIT_FAILURE);
    }

    hWndGraph = CreateWindowEx(
                                    WS_EX_APPWINDOW,
                                    lpszGraphClsName,
                                    lpszGraphCaption,
                                    WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    (HWND)hPrentWnd,
                                    (HMENU)NULL,
                                    hInstance,
                                    NULL
                                );

    CHECK_ERROR(hWndGraph == NULL,"Not able to create the handle to window","CreateWindowEx()");

    return hWndGraph;
}

int CreateLineGraph(HWND hWnd,LPSIMPLELINEGRAPH* OutSimpleLineGraph,int** ppPoints,int m,int* nPSize,int scale,LPCOLORREF pColor)
{
    
    (*OutSimpleLineGraph) = (LPSIMPLELINEGRAPH) calloc(1,sizeof(SIMPLELINEGRAPH));

    (*OutSimpleLineGraph)->Lines = (LPLINE*) calloc(m,sizeof(LPLINE));
    (*OutSimpleLineGraph)->nSize = m;
    (*OutSimpleLineGraph)->lpCoOrdinateAxis = CreateCoOrdinateAxis();

    glpSimpleGraph = (*OutSimpleLineGraph);

    COORDINATEAXIS tmpCoOrdinateAxis;

    tmpCoOrdinateAxis.scale = scale;

    SetCoOrdinateParamters((*OutSimpleLineGraph)->lpCoOrdinateAxis,&tmpCoOrdinateAxis,AX_SCALE);

    LINE tmpLine;

    HDC hdc = GetDC(hWnd);

    int RESY = GetDeviceCaps(hdc,LOGPIXELSY);

    for(int i=0;i<m;i++)
    {
        tmpLine.pPoints = (LPPOINT) calloc(nPSize[i],sizeof(POINT));
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
    
    HDC hdc;
    PAINTSTRUCT ps;
    

    switch(uMsg)
    {
        case WM_CREATE:
        break;

        case WM_PAINT:

        hdc = BeginPaint(hWnd,&ps);

        DrawLine(hdc,&ps,&(glpSimpleGraph->Lines[0]),NULL,NULL);

        EndPaint(hWnd,&ps);
        break;

        case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        break;
    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

status DestorySimpleLineGraph(LPSIMPLELINEGRAPH lpSimpleLineGraph)
{
    if(lpSimpleLineGraph == NULL)
        return FAILURE;
    
    for(int i=0;i < lpSimpleLineGraph->nSize;i++)
    {
        DestoryLine(&(lpSimpleLineGraph->Lines[i]));
    }

    lpSimpleLineGraph->Lines = NULL;
    lpSimpleLineGraph = NULL;

    return SUCCESS;
}