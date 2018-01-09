//=============================================================================
//
// ファイル名：graphics.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#pragma once

#include <d3dx9.h>

HRESULT InitD3D(HWND hWnd, bool bWindowMode);
void UninitD3D(void);
LPDIRECT3DDEVICE9 GetDevice(void);
bool ResetDevice(void);
bool SetWindowModeD3D(bool windowMode);
