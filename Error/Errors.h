#pragma once

#include<Windows.h>
#define CHECK_ERROR(cond,message,orgin)     do\
                                            {\
                                                if(cond)\
                                                {\
                                                    MessageBox((HWND)NULL,TEXT(message),TEXT("cause"),MB_ICONERROR);\
                                                    ExitProcess(EXIT_FAILURE);\
                                                }\
                                            }while(0)

#define SUCCESS            1
#define FAIL               0

HWND Init(HINSTANCE hInstance,HWND hParentWnd,LRESULT( __stdcall *)(HWND,UINT,WPARAM,LPARAM),PTCHAR className,PTCHAR windowCaption);

PVOID* xcalloc(size_t size);