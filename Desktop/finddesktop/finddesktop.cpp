﻿#include"finddesktop.h"
HWND WorkerW=NULL;

HWND findDesktopIconWnd()
{
    HWND hWorkerW = NULL;//WorkerW的句柄
    HWND hDefView = NULL;//SHELLDLL_DefView的句柄

    //找到WorkerW类的窗口
    hWorkerW = FindWindowEx(NULL, NULL, L"WorkerW", NULL);
    //通过遍历找到包含SHELLDLL_DefView的WorkerW
    while ((!hDefView) && hWorkerW)
    {
        hDefView = FindWindowEx(hWorkerW, NULL, L"SHELLDLL_DefView", NULL);
        WorkerW=hWorkerW;//得到WorkerW
        hWorkerW = FindWindowEx(NULL, hWorkerW, L"WorkerW", NULL);
    }
    //隐藏窗口，不让mainwindow被挡住
    ShowWindow(hWorkerW,0);
    return FindWindow(L"Progman",NULL);
}
void SendMessageToDesktop()
{
     PDWORD_PTR result = NULL;
     //发送消息，召唤WorkerW
     SendMessageTimeout(FindWindow(L"Progman",NULL), 0x52c, 0, 0, SMTO_NORMAL, 1000, result);
}

HWND GetWorkerW()
{
    findDesktopIconWnd();
    return WorkerW; //得到WorkerW
}
