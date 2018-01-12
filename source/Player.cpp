#include "Player.h"
#include "Camera.h"
#include "Input.h"

void MovePlayer(Player *_this);
void JumpPlayer(Player *_this);

void InitPlayer(Player *_this)
{
	// トランスフォームの初期設定
	InitTransform(&_this->transform);
	_this->transform.child = &_this->model.transform;
	_this->transform.pos.y = 100.0f;
	_this->last_pos = _this->model.transform.pos;

	// リジッドボディの初期設定
	InitRigidbody(&_this->rigidbody, &_this->transform);
	_this->rigidbody.bGravity = true;

	_this->targetAngle = Deg2Rad(-90.0f);

	InitSkinnedModel(&_this->model, "player.x");
	_this->model.alphaTestEnable = true;
	_this->model.transform.rot.y = Deg2Rad(-90.0f);

	_this->state = PlayerStateIdle;

	GetCamera()->target = &_this->transform.pos;
}

void UpdatePlayer(Player *_this)
{
	MovePlayer(_this);
	JumpPlayer(_this);

	_this->last_pos = _this->rigidbody.pos;

	UpdateRigidbody(&_this->rigidbody);

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

}

void DrawPlayer(Player *_this)
{
	UpdateTransformMtx(&_this->transform);

	DrawSkinnedModel(&_this->model);
}

void SetPlayerState(Player * _this, PlayerState state)
{
	switch (_this->state)
	{
	case PlayerStateIdle:
		switch (state)
		{
		case PlayerStateRunning:
			_this->model.animator->SetTrackAnimationSet(0, _this->model.animationSet[1]);
			_this->model.animator->SetTrackPosition(0, 0);
			_this->state = state;
			break;
		}
		break;
	case PlayerStateRunning:
		switch (state)
		{
		case PlayerStateIdle:
			_this->model.animator->SetTrackAnimationSet(0, _this->model.animationSet[0]);
			_this->model.animator->SetTrackPosition(0, 0);
			_this->state = state;
			break;
		}
		break;

	}
}

void UninitPlayer(Player *_this)
{
	UninitSkinnedModel(&_this->model);
}

void MovePlayer(Player *_this)
{

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
			_this->rigidbody.pos += move*2.0f;

			// 回転角度の計算
			//_this->targetAngle = -atan2(move.z, move.x);
			t = asinf(_this->dir.x * move.z - _this->dir.z * move.x);
			_this->targetAngle = _this->transform.rot.y - t;

			SetPlayerState(_this, PlayerStateRunning);

		}
		else
		{
			SetPlayerState(_this, PlayerStateIdle);
		}

	}

	_this->transform.rot.y = _this->transform.rot.y + (_this->targetAngle - _this->transform.rot.y) * 0.2f;

	//// 向きベクトルの更新
	_this->dir.x = cosf(_this->transform.rot.y);
	_this->dir.z = -sinf(_this->transform.rot.y);


}

void JumpPlayer(Player * _this)
{
	if (GetKeyboardTrigger(DIK_SPACE))
		_this->rigidbody.vel.y = 100.0f;
}
