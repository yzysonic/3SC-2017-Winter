#include "Player.h"
#include "Camera.h"
#include "Input.h"

void MovePlayer(Player *_this);


void InitPlayer(Player *_this)
{
	// ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€‚Ì‰ŠúÝ’è
	InitTransform(&_this->transform);
	_this->transform.child = &_this->model.transform;

	_this->targetAngle = 0.0f;

	InitSkinnedModel(&_this->model, "player.x");
	_this->model.alphaTestEnable = true;
	_this->model.transform.rot.y = Deg2Rad(-90.0f);

	GetCamera()->target = &_this->transform.pos;
}

void UpdatePlayer(Player *_this)
{
	MovePlayer(_this);

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

void UninitPlayer(Player *_this)
{
	UninitSkinnedModel(&_this->model);
}

void MovePlayer(Player *_this)
{

	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
	Camera* camera = GetCamera();

	float t = 0;

	// “ü—Íˆ—
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
	//		// ³‹K‰»
	//		D3DXVec3Normalize(&move, &move);

	//		// ˆÚ“®ˆ—
	//		_this->transform.pos += _this->dir * move.z * 5.0f;

	//		// ‰ñ“]ˆ—
	//		_this->targetAngle += move.x * 0.05f;
	//	}
	//}
	//else
	{

		if (move.x != 0.0f || move.z != 0.0f)
		{
			// ³‹K‰»
			move = move.normalized();

			float x = -move.x*cosf(-camera->phi + 0.5f*PI) - move.z*sinf(-camera->phi + 0.5f*PI);
			move.z = move.x*sinf(-camera->phi + 0.5f*PI) - move.z*cosf(-camera->phi + 0.5f*PI);
			move.x = x;

			// ˆÚ“®ˆ—
			_this->transform.pos += move*5.0f;

			// ‰ñ“]Šp“x‚ÌŒvŽZ
			//_this->targetAngle = -atan2(move.z, move.x);
			t = asinf(_this->dir.x * move.z - _this->dir.z * move.x);
			_this->targetAngle = _this->transform.rot.y - t;

		}

	}

	_this->transform.rot.y = _this->transform.rot.y + (_this->targetAngle - _this->transform.rot.y) * 0.2f;

	//// Œü‚«ƒxƒNƒgƒ‹‚ÌXV
	_this->dir.x = cosf(_this->transform.rot.y);
	_this->dir.z = -sinf(_this->transform.rot.y);


}
