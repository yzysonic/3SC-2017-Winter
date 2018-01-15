#include "SceneTitle.h"
#include "Core.h"

RectPolygon2D g_title_bk;

void InitSceneTitle(void)
{
	InitRectPolygon2D(&g_title_bk, GetTexture("title"));
	g_title_bk.transform.pos.x = 0.5f* SystemParameters::ResolutionX;
	g_title_bk.transform.pos.y = 0.5f* SystemParameters::ResolutionY;
}

void UpdateSceneTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
		SetScene(SceneGame);
}

void DrawSceneTitle(void)
{
	DrawRectPolygon2D(&g_title_bk);
}

void UninitSceneTitle(void)
{
	UninitRectPolygon2D(&g_title_bk);
}
