#include "SceneGame.h"
#include "Core.h"
#include "Light.h"
#include "Camera.h"
#include "Player.h"

Player g_player;

void InitSceneGame(void)
{
	InitLight();
	InitCamera();
	InitPlayer(&g_player);
}

void UpdateSceneGame(void)
{
	UpdateCamera();
	UpdatePlayer(&g_player);
}

void DrawSceneGame(void)
{
	DrawCamera();
	DrawPlayer(&g_player);
}

void UninitSceneGame(void)
{
	UninitPlayer(&g_player);
}
