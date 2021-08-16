#include<Windows.h>
#include "../SimpleBarGraph/SimpleBarGraph.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

// LPVECBARS lpVecBars;

// LPCOORDINATEAXIS coOrdinate;
// COORDINATEAXIS tmpCoOrdinateAxis;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd){

    LPSIMPLEBARGRAPH lpSimpleBarGraph = NULL;
    // LPSIMPLEBARGRAPH lpSimpleBarGraph2 = NULL;
    

    static TCHAR szWndClassName[] = TEXT("This is class name");
    static TCHAR szWndCaption[] = TEXT("Parent Window !!!");

    HBRUSH hBrush= NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX wndEx;
    MSG Msg;

    ZeroMemory(&wndEx,sizeof(WNDCLASSEX));
    ZeroMemory(&Msg,sizeof(MSG));

    HWND hWnd = NULL;

    hBrush = (HBRUSH) GetStockObject(WHITE_BRUSH);
    if(hBrush == NULL){
        MessageBox((HWND)NULL,TEXT("Not able to allocate the brush"),TEXT("GetStockObject()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    hCursor = LoadCursor((HINSTANCE)NULL,IDC_ARROW);
    if(hCursor == NULL){
        MessageBox((HWND)NULL,TEXT("Not able to allocate the cursor"),TEXT("LoadCursor()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    hIcon = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    if(hIcon == NULL){
        MessageBox((HWND)NULL,TEXT("Not able to allocate the icon"),TEXT("LoadIcon()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    hIconSm = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    if(hIconSm == NULL){
        MessageBox((HWND)NULL,TEXT("Not able to allocate the icon"),TEXT("LoadIcon()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    wndEx.cbSize = sizeof(wndEx);
    wndEx.cbWndExtra = 0;
    wndEx.cbClsExtra = 0;

    wndEx.hbrBackground = hBrush;
    wndEx.hCursor = hCursor;
    wndEx.hIcon = hIcon;
    wndEx.hIconSm = hIconSm;

    wndEx.lpfnWndProc = WndProc;
    wndEx.hInstance = hInstance;
    wndEx.lpszClassName = szWndClassName;
    wndEx.style = CS_HREDRAW | CS_VREDRAW;
    wndEx.lpszMenuName = NULL;
    
    if(!RegisterClassEx(&wndEx)){
        MessageBox((HWND)NULL,TEXT("Not able to register the callback"),TEXT("RegsiterClass()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }

    // unit *arr = (unit*) malloc(2 * sizeof(unit));

    // arr[0] = 50;
    // arr[1] = 50;

    // LPEXTRA extra = SetExtras(arr,2,0,2);
    // UpdateHeight(lpVecBars,BR_HEIGHT_MULTI,0,extra);
    
    // DestroyExtra(extra);
    // extra = NULL;



    hWnd = CreateWindowEx(
                            WS_EX_APPWINDOW,
                            szWndClassName,
                            szWndCaption,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            (HWND)NULL,
                            (HMENU)NULL,
                            hInstance,
                            NULL
                        );

    if(hWnd == NULL){
        MessageBox((HWND)NULL,TEXT("Not able to create window"),TEXT("CreateWindowEx()"),MB_ICONERROR);
        return EXIT_FAILURE;
    }


    ShowWindow(hWnd,nShowCmd);
    UpdateWindow(hWnd);

    HWND hWnd1 = Init(hInstance,hWnd);
    // HWND hWnd2 = Init(hInstance,hWnd);

    unit* heights = (unit*) calloc(12,sizeof(unit));
    LPCOLORREF lpColor = (LPCOLORREF) calloc(12,sizeof(COLORREF));
    CHAR* lables[] = {
        "2000",
        "2001",
        "2002",
        "2003",
        "2004",
        "2005",
        "2006",
        "2007",
        "2008",
        "2009",
        "2010",
        "2011"
    };
    heights[0] = 200;
    heights[1] = 60;
    heights[2] = 80;
    heights[3] = 20;

    heights[4] = 120;
    heights[5] = 100;

    heights[6] = 80;
    heights[7] = 60;
    heights[8] = 40;
    heights[9] = 20;

    heights[10] = 40;
    heights[11] = 20;

    lpColor[0] = RGB(255,255,0);
    lpColor[1] = RGB(255,45,90);
    lpColor[2] = RGB(255,218,185);
    lpColor[3] = RGB(123,104,238);

    lpColor[4] = RGB(176,224,230);
    lpColor[5] = RGB(72,61,139);

    lpColor[6] = RGB(255,255,0);
    lpColor[7] = RGB(255,45,90);
    lpColor[8] = RGB(255,218,185);
    lpColor[9] = RGB(123,104,238);

    lpColor[10] = RGB(176,224,230);
    lpColor[11] = RGB(72,61,139);

    CreateBarGraph(
                    hWnd1,
                    &lpSimpleBarGraph,
                    heights,lpColor,
                    20,
                    12,
                    "Students Attendance",
                    "YTitle",
                    lables
                );
    // CreateBarGraph(hWnd2,&lpSimpleBarGraph2,(heights + 4),(lpColor + 4),2);
    free(heights);
    
    while(GetMessage(&Msg,(HWND)NULL,0,0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }


    DestroySimpleBarGraph(lpSimpleBarGraph);
    // DestroySimpleBarGraph(lpSimpleBarGraph2);

    return Msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){

    HDC hdc = NULL;
    PAINTSTRUCT ps;

    static int cxClient,cyClient;

    switch(uMsg){

        case WM_SIZE:

        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;

        case WM_PAINT:

        hdc = BeginPaint(hWnd,&ps);

        SetTextAlign(hdc,TA_CENTER);
        TextOut(hdc,cxClient/2,cyClient/2,TEXT("This is Parent Window"),strlen("This is Parent Window"));

        EndPaint(hWnd,&ps);
        hdc = NULL;
        break;

        case WM_DESTROY :
        PostQuitMessage(EXIT_SUCCESS);
        break;

    }

    return(DefWindowProc(hWnd,uMsg,wParam,lParam));
}