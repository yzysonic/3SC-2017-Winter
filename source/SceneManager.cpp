#include "SceneManager.h"
#include "SceneGlobal.h"
#include "SceneGame.h"

void UninitCurrentScene(void);

static Scene g_current_scene = SceneNone;

void InitScene(void)
{
	// グローバルシーンの初期化
	InitSceneGlobal();
}

void UpdateScene(void)
{
	// グローバルシーンの更新
	UpdateSceneGlobal();

	// カレントシーンの更新
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		UpdateSceneGame();
		break;
	case SceneResult:

		break;
	}
}

void DrawScene(void)
{
	// グローバルシーンの描画
	DrawSceneGlobal();

	// カレントシーンの描画
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		DrawSceneGame();
		break;
	case SceneResult:

		break;
	}

}

void SetScene(Scene scene)
{
	// カレントシーンの終了処理
	UninitCurrentScene();

	// 新しいシーンの初期化
	switch (scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		InitSceneGame();
		break;
	case SceneResult:

		break;
	}

	g_current_scene = scene;
}

void UninitScene(void)
{
	UninitCurrentScene();	// カレントシーンの終了処理
	UninitSceneGlobal();	// グローバルシーンの終了処理
}

void UninitCurrentScene(void)
{
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		UninitSceneGame();
		break;
	case SceneResult:

		break;
	}

	g_current_scene = SceneNone;
}
