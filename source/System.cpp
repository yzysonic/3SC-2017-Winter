#include "System.h"
#include "Direct3D.h"
#include "Window.h"
#include "Input.h"
#include "Sound.h"


//=============================================================================
// システムの初期化処理
//=============================================================================
HRESULT InitSystem()
{
	HRESULT hr;

	// ウィンドウの初期化
	hr = InitWindow();

	// グラフィックスの初期化
	if (SUCCEEDED(hr))
		hr = InitD3D(GetHWnd(), GetWindowMode());

	// インプットの初期化
	if (SUCCEEDED(hr))
		hr = InitInput(GetHInstance(), GetHWnd());

	// サウンドの初期化
	if (SUCCEEDED(hr))
		hr = InitSound(GetHWnd());

	return hr;
}

//=============================================================================
// システムの終了処理
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	UninitD3D();
	UninitWindow();
}