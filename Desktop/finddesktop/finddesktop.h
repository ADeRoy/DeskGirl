#ifndef FINDDESKTOP_H
#define FINDDESKTOP_H
#include <qt_windows.h>

HWND findDesktopIconWnd();//找到桌面句柄
void SendMessageToDesktop();//给桌面Program Manager 发送0x052c消息，解除父子关系
HWND GetWorkerW();          //得到WorkerW

#endif // FINDDESKTOP_H
