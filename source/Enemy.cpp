#include "Enemy.h"

#define EnemyMaxFrame	(23)

Model g_pModelEnemy[EnemyMaxFrame];
Model g_sphere;
Enemy g_enemyList[EnemyMax];

void InitEnemyManager(void)
{
	for (int i = 0; i < EnemyMaxFrame; i++)
	{
		char fileName[128];
		sprintf(fileName, "enemy_%02d.x", i);
		InitModel(&g_pModelEnemy[i], fileName);
	}
	InitModel(&g_sphere, "sphere.x");
	g_sphere.transform.scl = 5.0f*Vector3::one;
	g_sphere.transform.pos.y = 5.0f;
}

void UpdateEnemyManager(void)
{
	Enemy *enemy = g_enemyList;
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (enemy->active == false)
			continue;
		UpdateEnemy(enemy);
	}
}

void DrawEnemyManager(void)
{
	Enemy *enemy = g_enemyList;
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (enemy->active == true)
			DrawEnemy(enemy);
	}
}

void UninitEnemyManager(void)
{
	for (int i = 0; i < EnemyMaxFrame; i++)
	{
		UninitModel(&g_pModelEnemy[i]);
	}

	Enemy *enemy = g_enemyList;
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (enemy->active == true)
			UninitEnemy(enemy);
	}
	UninitModel(&g_sphere);
}

void SwapEnemy(Vector3 pos)
{
	Enemy *enemy = g_enemyList;
	for (int i = 0; i < EnemyMax; i++, enemy++)
	{
		if (enemy->active == false)
		{
			InitEnemy(enemy);
			enemy->rigidbody.pos = pos;
			return;
		}
	}


}

void InitEnemy(Enemy * _this)
{
	InitTransform(&_this->transform);
	InitRigidbody(&_this->rigidbody, &_this->transform);
	InitShadow(&_this->shadow, &_this->transform);
	_this->collider.radius = 5.0f;
	_this->shadow.transform.scl = Vector3::one * 0.6f;
	_this->active = true;
	_this->state = EnemyStateRunning;
	_this->animeFrame = 1;
}

void UpdateEnemy(Enemy * _this)
{
	if (GetKeyboardTrigger(DIK_E))
	{
		if (_this->state == EnemyStateIdle)
			SetEnemyState(_this, EnemyStateRunning);
		else
			SetEnemyState(_this, EnemyStateIdle);
	}

	if (_this->state == EnemyStateRunning)
	{
		if (_this->timer > 1 / 60.0f)
		{
			_this->animeFrame = (_this->animeFrame+1) % (EnemyMaxFrame - 1) + 1;
			_this->timer = 0.0f;
		}
		_this->timer += DeltaTime();
	}

	//_this->rigidbody.pos.z += -0.3f;
	//if (_this->rigidbody.pos.z <= -100.1f)
	//	_this->rigidbody.pos.z += 100.0f;

	UpdateRigidbody(&_this->rigidbody);

	_this->collider.pos = Vector3(0.0f, 5.0f, 0.0f) + _this->transform.pos;
}

void DrawEnemy(Enemy * _this)
{
	//if (_this->state == EnemyStateRunning)
	//	_this->animeFrame = (_this->animeFrame + 1) % (EnemyMaxFrame - 1) + 1;

	_this->transform.child->sibling = &g_pModelEnemy[_this->animeFrame].transform;
	UpdateTransformMtx(&_this->transform);

	DrawModel(&g_pModelEnemy[_this->animeFrame]);

	UpdateTransformMtx(&g_sphere.transform, &_this->transform.mtxWorld);
	DrawModel(&g_sphere);
}

void SetEnemyState(Enemy * _this, EnemyState state)
{
	switch (_this->state)
	{
	case EnemyStateIdle:
		if (state == EnemyStateRunning)
			_this->state = EnemyStateRunning;
		break;

	case EnemyStateRunning:
		switch (state)
		{
		case EnemyStateAttacking:
			_this->state = EnemyStateAttacking;
			break;
		case EnemyStateIdle:
			_this->animeFrame = 0;
			_this->state = EnemyStateIdle;
			break;
		}
			
		break;

	case EnemyStateAttacking:
		if (state == EnemyStateRunning)
			state = EnemyStateRunning;
		break;
	}
}

Enemy * GetEnemyList(void)
{
	return g_enemyList;
}

void UninitEnemy(Enemy * _this)
{
	UninitShadow(&_this->shadow);
}
