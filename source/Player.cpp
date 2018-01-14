#include "Player.h"
#include "Camera.h"
#include "Input.h"

void MovePlayer(Player *_this);
void SetIdlePlayer(Player *_this);
void SetJumpPlayer(Player *_this);
void SetKickPlayer(Player *_this);

void InitPlayer(Player *_this)
{
	// トランスフォームの初期化
	InitTransform(&_this->transform);
	_this->transform.pos = Vector3(-20.0f, 50.0f, -20.0f);
	AddTransformChild(&_this->transform, &_this->model.transform);
	GetCamera()->target = &_this->transform.pos;

	// リジッドボディの初期化
	InitRigidbody(&_this->rigidbody, &_this->transform);
	_this->rigidbody.bGravity = true;

	// モデルの初期化
	InitSkinnedModel(&_this->model, "player.x");
	_this->model.alphaTestEnable = true;
	_this->model.transform.rot.y = Deg2Rad(-90.0f);

	// 影の初期化
	InitShadow(&_this->shadow, &_this->transform);

	// 足のコライダーの初期化
	InitSphereCollider(&_this->collider_foot, &_this->transform.pos);
	_this->collider_foot.radius = 7.0f;

	// 他の初期化
	_this->targetAngle = Deg2Rad(-90.0f);
	SetIdlePlayer(_this);
	_this->state_timer = 0.0f;
	_this->isMoveable = true;

	InitModel(&_this->sphere, "sphere.x");
	_this->sphere.transform.scl = 7.0f*Vector3::one;
	_this->bone_foot = (Bone*)FindFrameByName("Armature_mixamorig_RightFoot", _this->model.rootFrame);


}

void UpdatePlayer(Player *_this)
{
	if (GetKeyboardTrigger(DIK_K))
		SetPlayerState(_this, PlayerStateKick);

	if (GetKeyboardTrigger(DIK_SPACE))
		SetPlayerState(_this, PlayerStateJumping);

	if (GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_A) ||
		GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_D))
	{
		SetPlayerState(_this, PlayerStateRunning);
	}


	switch (_this->state)
	{
	case PlayerStateIdle:
		break;
	case PlayerStateRunning:
		MovePlayer(_this);
		break;
	case PlayerStateJumping:
		MovePlayer(_this);
		break;
	case PlayerStateKick:
		if (_this->state_timer >= 0.3f)
			SetPlayerState(_this, PlayerStateIdle);
		_this->state_timer += DeltaTime();
		break;
	}

	UpdateRigidbody(&_this->rigidbody);

	if (_this->transform.pos.y <= 20.0f)
		SetPlayerState(_this, PlayerStateOutOfField);


	if (GetKeyboardTrigger(DIK_PGUP))
	{
		if (_this->model.activeAnimation + 1 < (int)_this->model.animator->GetMaxNumAnimationSets())
		{
			_this->model.animator->SetTrackAnimationSet(0, _this->model.animationSet[++_this->model.activeAnimation]);
			_this->model.animator->SetTrackPosition(0, 0);
		}
	}
		
	if (GetKeyboardTrigger(DIK_PGDN))
	{
		if (_this->model.activeAnimation - 1 >= 0)
		{
			_this->model.animator->SetTrackAnimationSet(0, _this->model.animationSet[--_this->model.activeAnimation]);
			_this->model.animator->SetTrackPosition(0, 0);
		}
	}
	D3DXVECTOR4 foot_pos;
	D3DXVec3Transform(&foot_pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &_this->bone_foot->mtxCombined);
	_this->collider_foot.pos.x = foot_pos.x;
	_this->collider_foot.pos.y = foot_pos.y;
	_this->collider_foot.pos.z = foot_pos.z;

	UpdateTransformMtx(&_this->sphere.transform, &_this->bone_foot->mtxCombined);
}

void DrawPlayer(Player *_this)
{
	UpdateTransformMtx(&_this->transform);

	DrawSkinnedModel(&_this->model);
}

void SetPlayerState(Player * _this, PlayerState state)
{
	if (state == PlayerStateOutOfField)
	{
		if (_this->state == PlayerStateOutOfField)
			return;
		SetSkinnedModelAnime(&_this->model, PlayerStateIdle);
		_this->isMoveable = false;
		_this->state = PlayerStateOutOfField;
		return;
	}

	switch (_this->state)
	{
	case PlayerStateIdle:
		switch (state)
		{
		case PlayerStateRunning:
			SetSkinnedModelAnime(&_this->model, PlayerStateRunning);
			break;
		case PlayerStateJumping:
			SetJumpPlayer(_this);
			break;
		case PlayerStateKick:
			SetKickPlayer(_this);
			break;
		default:
			return;
		}
		break;

	case PlayerStateRunning:
		switch (state)
		{
		case PlayerStateIdle:
			SetIdlePlayer(_this);
			break;
		case PlayerStateJumping:
			SetJumpPlayer(_this);
			break;
		case PlayerStateKick:
			SetKickPlayer(_this);
			break;
		default:
			return;
		}
		break;

	case PlayerStateJumping:
		switch (state)
		{
		case PlayerStateIdle:
			SetIdlePlayer(_this);
			break;
		default:
			return;
		}

	case PlayerStateKick:
		switch (state)
		{
		case PlayerStateIdle:
			SetIdlePlayer(_this);
			break;
		default:
			return;
		}
		break;

		break;

	case PlayerStateDeath:

		break;
	default:
		return;
	}

	_this->state = state;
}

void UninitPlayer(Player *_this)
{
	UninitSkinnedModel(&_this->model);
	UninitShadow(&_this->shadow);
	UnintSphereCollider(&_this->collider_foot);
	UninitModel(&_this->sphere);
}

void MovePlayer(Player *_this)
{
	if (!_this->isMoveable)
		return;

	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
	Camera* camera = GetCamera();

	float t = 0;

	// 入力処理
	if (GetKeyboardPress(DIK_W))
		move.z += 1.0f;
	if (GetKeyboardPress(DIK_S))
		move.z += -1.0f;
	if (GetKeyboardPress(DIK_A))
		move.x += -1.0f;
	if (GetKeyboardPress(DIK_D))
		move.x += 1.0f;


	//if (camera->fps_mode)
	//{
	//	_this->dir.x = cosf(_this->transform.rot.y);
	//	_this->dir.z = -sinf(_this->transform.rot.y);

	//	if (move.x != 0.0f || move.z != 0.0f)
	//	{
	//		// 正規化
	//		D3DXVec3Normalize(&move, &move);

	//		// 移動処理
	//		_this->transform.pos += _this->dir * move.z * 5.0f;

	//		// 回転処理
	//		_this->targetAngle += move.x * 0.05f;
	//	}
	//}
	//else
	{

		if (move.x != 0.0f || move.z != 0.0f)
		{
			// 正規化
			move = move.normalized();

			float x = -move.x*cosf(-camera->phi + 0.5f*PI) - move.z*sinf(-camera->phi + 0.5f*PI);
			move.z = move.x*sinf(-camera->phi + 0.5f*PI) - move.z*cosf(-camera->phi + 0.5f*PI);
			move.x = x;

			// 移動処理
			//_this->rigidbody.pos += move*2.0f;

			// 回転角度の計算
			//_this->targetAngle = -atan2(move.z, move.x);
			t = asinf(_this->dir.x * move.z - _this->dir.z * move.x);
			_this->targetAngle = _this->transform.rot.y - t;

			//SetPlayerState(_this, PlayerStateRunning);

		}
		else if(_this->state == PlayerStateRunning)
		{
			SetPlayerState(_this, PlayerStateIdle);
		}

		_this->rigidbody.vel.x = move.x*120.0f;
		_this->rigidbody.vel.z = move.z*120.0f;


	}

	_this->transform.rot.y = _this->transform.rot.y + (_this->targetAngle - _this->transform.rot.y) * 0.2f;

	// 向きベクトルの更新
	_this->dir.x = cosf(_this->transform.rot.y);
	_this->dir.z = -sinf(_this->transform.rot.y);


}

void SetIdlePlayer(Player * _this)
{
	SetSkinnedModelAnime(&_this->model, PlayerStateIdle);
	_this->rigidbody.vel.x =
	_this->rigidbody.vel.z = 0.0f;
	_this->collider_foot.active = false;
}

void SetJumpPlayer(Player * _this)
{
	SetSkinnedModelAnime(&_this->model, PlayerStateJumping);
	_this->model.animator->SetTrackPosition(0, 0.592f);
	_this->rigidbody.vel.y = 50.0f;
}

void SetKickPlayer(Player * _this)
{
	SetSkinnedModelAnime(&_this->model, PlayerStateKick);
	_this->state_timer = 0.0f;
	_this->collider_foot.active = true;
}
