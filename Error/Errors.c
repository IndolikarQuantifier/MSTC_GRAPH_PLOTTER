#include "Errors.h"

HWND Init(HINSTANCE hInstance,HWND hPrentWnd,LRESULT( __stdcall *WndProc)(HWND,UINT,WPARAM,LPARAM),PTCHAR className,PTCHAR windowCaption)
{
    // static TCHAR lpszGraphClsName[] = TEXT("BarGraph");
    // static TCHAR lpszGraphCaption[] = TEXT("SimpleBar Graph !!!");

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

    wndGraphEx.lpfnWndProc = WndProc;
    wndGraphEx.lpszClassName = className;
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
                                    className,
                                    windowCaption,
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

PVOID* xcalloc(size_t size)
{
    PVOID newMemory = calloc(1,size);
    if(newMemory == NULL)
    {
        MessageBox((HWND)NULL,TEXT("Not able to allocate the memory !!!"),TEXT("xcalloc()"),MB_ICONERROR);
        ExitProcess(GetLastError());
    }

    return newMemory;
}