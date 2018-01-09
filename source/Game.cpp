#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Random.h"
#include "Texture.h"

// グローバル変数
bool g_bRunGame = true;

// ゲーム初期化
void InitGame()
{
	InitRenderer();
	InitTime();
	InitRandom();
	InitScene();
}

// ゲームループ
void RunGame(void)
{
	while (g_bRunGame)
	{
		CheckMesg();
		UpdateInput();
		UpdateScene();
		DrawFrame();
		FramerateControl();
	}
}

// ゲーム終了処理
void UninitGame(void)
{
	UninitScene();
	UninitRenderer();
}

// ゲーム終了確認
bool EndGame(void)
{
	return true;
}

// ゲーム終了
void StopGame(void)
{
	g_bRunGame = false;
}
