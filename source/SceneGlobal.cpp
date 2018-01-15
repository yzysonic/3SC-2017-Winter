#include "SceneGlobal.h"
#include "Core.h"
#include "skybox.h"

RectPolygon2D g_loading;

void InitSceneGlobal(void)
{
	SetGravity(Vector3(0.0f, -170.0f, 0.0f));
	g_loading.transform.pos.x = SystemParameters::ResolutionX - 227;
	g_loading.transform.pos.y = SystemParameters::ResolutionY - 62;
	LoadSkyboxTexture();
}

void UpdateSceneGlobal(void)
{
}

void DrawSceneGlobal(void)
{
}

void UninitSceneGlobal(void)
{
	UninitRectPolygon2D(&g_loading);
	ReleaseTextureAll();
}

void ShowLoading(bool value)
{
	if (value)
		SetRectPolygon2DOpacity(&g_loading, 1.0f);
	else
		SetRectPolygon2DOpacity(&g_loading, 0.0f);
}
