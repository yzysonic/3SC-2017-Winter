#pragma once

#include "Core.h"
#include "Shadow.h"

#define EnemyMax	(10)

enum EnemyState
{
	EnemyStateIdle,
	EnemyStateRunning,
	EnemyStateAttacking,
	EnemyStateMax
};

struct Enemy
{
	Transform transform;
	Rigidbody rigidbody;
	Shadow shadow;
	EnemyState state;
	SphereCollider collider;
	int animeFrame;
	float timer;
	bool active;
};

void InitEnemyManager(void);
void UpdateEnemyManager(void);
void DrawEnemyManager(void);
void UninitEnemyManager(void);

void SwapEnemy(Vector3 pos);

void InitEnemy(Enemy *_this);
void UpdateEnemy(Enemy *_this);
void DrawEnemy(Enemy *_this);
void SetEnemyState(Enemy *_this, EnemyState state);
Enemy *GetEnemyList(void);
void UninitEnemy(Enemy *_this);