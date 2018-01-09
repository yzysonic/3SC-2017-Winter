#pragma once

#include <Windows.h>

HRESULT InitWindow(void);
void UninitWindow(void);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void CheckMesg(void);
HWND GetHWnd(void);
HINSTANCE GetHInstance(void);
bool GetWindowMode(void);
void SetWindowMode(bool windowMode);