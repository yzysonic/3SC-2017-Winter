//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "System.h"
#include "Game.h"
#include "SceneManager.h"


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // メモリリーク自動検出
#endif

	SystemParameters::windowTitle = "除草大作戦";

	// 初期化
	if (FAILED(InitSystem()))
		return 1;

	// マウスを隠す
	//ShowCursor(false);

	// ゲーム初期化
	InitGame();
#ifndef _DEBUG
	SetScene(SceneTitle);
#else
	SetScene(SceneTitle);
#endif

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
}
