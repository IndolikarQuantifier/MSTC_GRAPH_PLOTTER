#include<Windows.h>
#include "../SimpleLineGraph/SimpleLineGraph.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd){

    LPSIMPLELINEGRAPH lpSimpleLineGraph = NULL;
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

    HWND hWnd1 = Init(hInstance,hWnd,WndSimpleLineGraphProc,"Line Graph","Wnd Line Caption");
    // HWND hWnd2 = Init(hInstance,hWnd);

    int* heights = (int*) calloc(16,sizeof(int));
    int length[4] = {4,4,6,2};
    LPCOLORREF lpColor = (LPCOLORREF) calloc(1,sizeof(COLORREF));
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

    heights[0] = 10;

    heights[1] = 20;

    heights[2] = 30;

    heights[3] = 100;

    // --- 

    heights[4] = 40;

    heights[5] = 20;

    heights[6] = 10;

    heights[7] = 70;

    // ---

    heights[8] = 40;

    heights[9] = 50;

    heights[10] = 40;

    heights[11] = 30;

    lpColor[0] = RGB(255,255,0);
    
    CreateLineGraph(
                    hWnd1,
                    &lpSimpleLineGraph,
                    heights,
                    12,
                    10,
                    lpColor,
                    "Years",
                    lables
                );
    // CreateBarGraph(hWnd2,&lpSimpleBarGraph2,(heights + 4),(lpColor + 4),2);
    free(heights);
    
    while(GetMessage(&Msg,(HWND)NULL,0,0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }


    DestorySimpleLineGraph(lpSimpleLineGraph);
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
        TextOut(hdc,cxClient/2,cyClient/2,TEXT("This is Line Plot"),strlen("This is Line Plot"));

        EndPaint(hWnd,&ps);
        hdc = NULL;
        break;

        case WM_DESTROY :
        PostQuitMessage(EXIT_SUCCESS);
        break;

    }

    return(DefWindowProc(hWnd,uMsg,wParam,lParam));
}