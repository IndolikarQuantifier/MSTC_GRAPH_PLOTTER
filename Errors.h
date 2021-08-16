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