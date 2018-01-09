//=============================================================================
//
// ファイル名：graphics.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "Direct3D.h"
#include "Common.h"
#include <DxErr.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")


LPDIRECT3D9				g_pD3D = NULL;
LPDIRECT3DDEVICE9		g_pDevice = NULL;
D3DPRESENT_PARAMETERS	g_d3dpp = {};
HWND					g_hWnd = NULL;


//=============================================================================
// グラフィックの初期化処理
//=============================================================================
HRESULT InitD3D(HWND hWnd, bool bWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SystemParameters::ResolutionX;	// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SystemParameters::ResolutionY;	// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.hDeviceWindow = hWnd;							// 使用するウィンド
	d3dpp.Windowed = bWindowMode;						// ウィンドウモードの設定
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
		D3DDEVTYPE_HAL,								// ディスプレイタイプ
		hWnd,										// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
		&d3dpp,										// デバイスのプレゼンテーションパラメータ
		&g_pDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	g_d3dpp = d3dpp;
	g_hWnd = hWnd;

	return S_OK;
}

//=============================================================================
// グラフィックの終了処理
//=============================================================================
void UninitD3D()
{
	SafeRelease(g_pDevice);
	SafeRelease(g_pD3D);
}


LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pDevice;
}

bool ResetDevice(void)
{
	HRESULT hr = g_pDevice->Reset(&g_d3dpp);

	if (FAILED(hr))
	{
		MessageBox(g_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		hr = g_pDevice->TestCooperativeLevel();
		MessageBox(g_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

bool SetWindowModeD3D(bool windowMode)
{
	g_d3dpp.Windowed = windowMode;

	// ウィンドウモード
	if (windowMode)
	{
		g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// バックバッファ
		g_d3dpp.FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	// フルスクリーンモード
	else
	{
		g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;						// バックバッファ
		g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	return ResetDevice();
}
