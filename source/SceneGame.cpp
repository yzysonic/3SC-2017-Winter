#include "SceneGame.h"
#include "Core.h"
#include "Light.h"
#include "Camera.h"
#include "skybox.h"
#include "Player.h"
#include "Island.h"

Player g_player;
Island g_island;
bool g_bShowCollider = false;

void CollisionTest(void);

void InitSceneGame(void)
{
	InitLight();
	InitCamera();
	InitSkybox();
	InitPlayer(&g_player);
	InitIsland(&g_island);
}

void UpdateSceneGame(void)
{
	if (GetKeyboardTrigger(DIK_C))
		g_bShowCollider = !g_bShowCollider;
	UpdateCamera();
	UpdatePlayer(&g_player);
	CollisionTest();
}

void DrawSceneGame(void)
{
	DrawCamera();
	DrawSkybox();
	DrawIsland(&g_island);
	if(g_bShowCollider)
		DrawMeshCollider(&g_island.collider);
	DrawPlayer(&g_player);
}

void UninitSceneGame(void)
{
	UninitSkybox();
	UninitPlayer(&g_player);
	UninitIsland(&g_island);
}

void CollisionTest(void)
{
	for (int i = 0; i < g_island.collider.polyNum; i++)
		g_island.collider.polyList[i].isCollidinig = false;

	for (int i = 0; i < g_island.collider.polyNum; i++)
	{
		Vector3 offset = CollisionTestPolyAndLine(g_island.collider.polyList[i], g_player.last_pos, (g_player.transform.pos - g_player.last_pos));
		if (offset != Vector3::zero)
		{
			g_player.rigidbody.pos += offset;
			g_player.rigidbody.vel = Vector3::zero;
			g_island.collider.polyList[i].isCollidinig = true;
			return;
		}
	}
}
