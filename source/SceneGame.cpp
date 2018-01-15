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


void InitSceneGame(void)
{
	InitLight();
	InitCamera();
	InitSkybox();
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

	for (int i = 0; i < 10; i++)
		SwapEnemy(Vector3(Randomf(-130.f, 130.f), 23.0f, Randomf(-130.f, 130.f)));
}

void UpdateSceneGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_C))
		g_bShowCollider = !g_bShowCollider;
#endif
	UpdateCamera();
	UpdatePlayer(&g_player);
	UpdateEnemyManager();
	CollisionTest();
}

void DrawSceneGame(void)
{
	DrawCamera();
	DrawSkybox();
	DrawIsland(&g_island);
	DrawShadow();
#ifdef _DEBUG
	if (g_bShowCollider)
	{
		DrawMeshCollider(&g_ground);
		DrawMeshCollider(&g_island.collider);
		//DrawSphereCollider(&g_player.collider_foot);
		DrawModel(&g_player.sphere);
	}
#endif
	DrawPlayer(&g_player);
	DrawEnemyManager();
}

void UninitSceneGame(void)
{
	UninitSkybox();
	UninitPlayer(&g_player);
	UninitEnemyManager();
	UninitIsland(&g_island);
	UninitMeshCollider(&g_ground);
}

void CollisionTest(void)
{
	Collision collision;

	// プレイヤーと島と判定
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

	// プレイヤーと地面の判定
	if (CollisionTestMeshAndLine(g_ground, g_player.rigidbody.pos_last, g_player.rigidbody.move_last, &collision))
	{
		//CollisionResponse(&collision, &g_player.rigidbody);
		SetScene(SceneTitle);
		return;
	}

	// エネミーの判定
	Enemy* enemy = GetEnemyList();
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (!enemy->active) continue;

		// プレイヤーとの判定
		if (g_player.collider_foot.active)
		{
			if (CollisionTestSphere(enemy->collider, g_player.collider_foot, &collision))
			{
				enemy->rigidbody.bGravity = true;
				enemy->rigidbody.vel = (0.5f*collision.normal + g_player.dir*0.5f) * 250.0f;
				enemy->rigidbody.vel.y = fabsf(enemy->rigidbody.vel.y);
			}
		}

		// 島との判定
		if (CollisionTestMeshAndLine(g_island.collider, enemy->rigidbody.pos_last, enemy->rigidbody.move_last, &collision))
		{
			CollisionResponse(&collision, &enemy->rigidbody);
			enemy->rigidbody.pos.y += 0.01f;
			enemy->rigidbody.vel = Vector3::zero;
			enemy->rigidbody.bGravity = false;
		}
		

	}

}


