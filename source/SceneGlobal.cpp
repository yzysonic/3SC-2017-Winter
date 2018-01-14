#include "SceneGlobal.h"
#include "Core.h"

void InitSceneGlobal(void)
{
	SetGravity(Vector3(0.0f, -170.0f, 0.0f));
}

void UpdateSceneGlobal(void)
{
}

void DrawSceneGlobal(void)
{
}

void UninitSceneGlobal(void)
{
	ReleaseTextureAll();
}
