#include "SceneGame.h"
#include "Core.h"
#include "Light.h"
#include "Camera.h"
#include "skybox.h"
#include "Player.h"
#include "Enemy.h"
#include "Island.h"
#include "Shadow.h"

Player g_player;
Island g_island;
MeshCollider g_ground;
bool g_bShowCollider = false;

void CollisionTest(void);
bool CollisionTestPlayer(void);


void InitSceneGame(void)
{
	InitLight();
	InitCamera();
	//InitSkybox();
	InitPlayer(&g_player);
	InitEnemyManager();
	InitIsland(&g_island);

	g_ground.polyNum = 2;
	g_ground.polyList = _newarray(PolyCollider, 2);
	g_ground.polyList[0].vtx[0] = Vector3(-1000.0f, -10.0f, 1000.0f);
	g_ground.polyList[0].vtx[1] = Vector3( 1000.0f, -10.0f, 1000.0f);
	g_ground.polyList[0].vtx[2] = Vector3(-1000.0f, -10.0f, -1000.0f);
	g_ground.polyList[1].vtx[2] = Vector3( 1000.0f, -10.0f, -1000.0f);
	g_ground.polyList[1].vtx[0] = g_ground.polyList[0].vtx[2];
	g_ground.polyList[1].vtx[1] = g_ground.polyList[0].vtx[1];
	g_ground.polyList[0].isCollidinig = false;
	g_ground.polyList[1].isCollidinig = false;
	CalulatePolyColliderNormal(g_ground.polyList);
	CalulatePolyColliderNormal(g_ground.polyList+1);

	SwapEnemy(Vector3(10.0f, 23.0f, 10.0f));
}

void UpdateSceneGame(void)
{
	if (GetKeyboardTrigger(DIK_C))
		g_bShowCollider = !g_bShowCollider;
	UpdateCamera();
	UpdatePlayer(&g_player);
	UpdateEnemyManager();
	CollisionTest();
}

void DrawSceneGame(void)
{
	DrawCamera();
	//DrawSkybox();
	DrawIsland(&g_island);
	DrawShadow();
	if (g_bShowCollider)
	{
		DrawMeshCollider(&g_ground);
		DrawMeshCollider(&g_island.collider);
		//DrawSphereCollider(&g_player.collider_foot);
		DrawModel(&g_player.sphere);
	}
	DrawPlayer(&g_player);
	DrawEnemyManager();
}

void UninitSceneGame(void)
{
	//UninitSkybox();
	UninitPlayer(&g_player);
	UninitEnemyManager();
	UninitIsland(&g_island);
	UninitMeshCollider(&g_ground);
}

void CollisionTest(void)
{
	CollisionTestPlayer();
}

bool CollisionTestPlayer(void)
{
	Collision collision;

	// “‡‚Æ‚Ì”»’è
	if (CollisionTestMeshAndLine(g_island.collider, g_player.rigidbody.pos_last, g_player.rigidbody.move_last, &collision))
	{
		if (collision.normal.y != 1.0f)
		{
			g_player.rigidbody.vel.x =
			g_player.rigidbody.vel.z = 0.0f;
		}

		CollisionResponse(&collision, &g_player.rigidbody);
		if (g_player.state == PlayerStateJumping)
			SetPlayerState(&g_player, PlayerStateIdle);
	}

	// ’n–Ê‚Æ‚Ì”»’è
	if (CollisionTestMeshAndLine(g_ground, g_player.rigidbody.pos_last, g_player.rigidbody.move_last, &collision))
		CollisionResponse(&collision, &g_player.rigidbody);

	Enemy* enemy = GetEnemyList();
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (!enemy->active) continue;
		if (!g_player.collider_foot.active) continue;

		if (CollisionTestSphere(enemy->collider, g_player.collider_foot, &collision))
		{
			enemy->rigidbody.bGravity = true;
			enemy->rigidbody.vel = collision.normal * 200.0f;
		}

	}

	return false;
}


