//=============================================================================
//
// �t�@�C�����Fgraphics.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <d3dx9.h>

HRESULT InitD3D(HWND hWnd, bool bWindowMode);
void UninitD3D(void);
LPDIRECT3DDEVICE9 GetDevice(void);
bool ResetDevice(void);
bool SetWindowModeD3D(bool windowMode);
