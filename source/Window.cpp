#include "Window.h"
#include "Common.h"
#include "Direct3D.h"
#include "Game.h"

static HINSTANCE g_hInstance = NULL;
static HWND g_hWnd = NULL;
static MSG  g_Msg;
static bool g_bWindowMode = true;
static char* g_ClassName = "MainWindow";

//=============================================================================
// Windowsの初期化処理
//=============================================================================
HRESULT InitWindow()
{
	g_hInstance = GetModuleHandle(NULL);
	int width = SystemParameters::ResolutionX;
	int height = SystemParameters::ResolutionY;

	// ウィンドウクラスを登録する
	WNDCLASSEX	wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = g_hInstance;
	wcex.lpszClassName = g_ClassName;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClassEx(&wcex);


	// ウィンドウオブジェクトを作成する.
	g_hWnd = CreateWindowEx(
		0,							//拡張スタイル（任意）
		g_ClassName,			//クラス名
		SystemParameters::windowTitle,				//ウィンドウ名（タイトル）
		(WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU),		//スタイル
		((GetSystemMetrics(SM_CXSCREEN) - width) / 2),		//横方向の位置
		((GetSystemMetrics(SM_CYSCREEN) - height) / 2),	//縦方向の位置
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//幅
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//高さ
		NULL,						//親ウィンドウ
		NULL,						//メニュー
		g_hInstance,				//アプリケーションインスタンスのハンドル
		NULL						//ウィンドウ作成データ
	);

	if (g_hWnd == NULL)
		return E_FAIL;

	//ウィンドウをスクリーンに表示する
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	return S_OK;
}


//=============================================================================
// Windowsの終了処理
//=============================================================================
void UninitWindow()
{
	// ウィンドウのデストロイド
	g_hWnd = NULL;

	// ウィンドウクラスの登録を解除
	UnregisterClass(g_ClassName, g_hInstance);
	g_hInstance = NULL;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		if (EndGame())
		{
			DestroyWindow(GetHWnd());	// ウィンドウを破棄するよう指示する
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]キーが押された
			StopGame();
			break;
		case VK_F9:			// [F9]キーが押された
			SetWindowMode(!g_bWindowMode);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CheckMesg(void)
{
	if (PeekMessage(&g_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()が呼ばれたらゲーム終了
		if (g_Msg.message == WM_QUIT)
		{
			StopGame();
			return;
		}
		// メッセージの翻訳とディスパッチ
		else
		{
			TranslateMessage(&g_Msg);
			DispatchMessage(&g_Msg);
		}
	}

}


HWND GetHWnd()
{
	return g_hWnd;
}

HINSTANCE GetHInstance(void)
{
	return g_hInstance;
}

bool GetWindowMode(void)
{
	return g_bWindowMode;
}

void SetWindowMode(bool windowMode)
{
	if (!SetWindowModeD3D(windowMode))
		return;

	if (windowMode)
	{
		SetWindowPos(g_hWnd, NULL,
			(GetSystemMetrics(SM_CXSCREEN) - SystemParameters::ResolutionX) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - SystemParameters::ResolutionY) / 2,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	g_bWindowMode = windowMode;
}