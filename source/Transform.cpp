#include "Transform.h"

void InitTransform(Transform * _this)
{
	_this->child = NULL;
	_this->pos = Vector3(0.0f, 0.0f, 0.0f);
	_this->rot = Vector3(0.0f, 0.0f, 0.0f);
	_this->scl = Vector3(1.0f, 1.0f, 1.0f);
	D3DXMatrixIdentity(&_this->mtxLocal);
	D3DXMatrixIdentity(&_this->mtxWorld);
}

void UpdateTransformMtx(Transform * _this, D3DXMATRIX *mtxParent)
{
	D3DXMATRIX temp;

	// ƒ[ƒJƒ‹‹óŠÔ‚Ì•ÏŠ·‚ð”½‰f
	_this->mtxWorld = _this->mtxLocal;

	// ƒXƒP[ƒ‹‚ð”½‰f
	D3DXMatrixScaling(&temp, _this->scl.x, _this->scl.y, _this->scl.z);
	_this->mtxWorld *= temp;

	// ‰ñ“]‚ð”½‰f
	D3DXMatrixRotationYawPitchRoll(&temp, _this->rot.y, _this->rot.x, _this->rot.z);
	_this->mtxWorld *= temp;

	// ˆÚ“®‚ð”½‰f
	D3DXMatrixTranslation(&temp, _this->pos.x, _this->pos.y, _this->pos.z);
	_this->mtxWorld *= temp;

	// e‹óŠÔ‚Ì•ÏŠ·‚ð”½‰f
	if (mtxParent)
		_this->mtxWorld *= *mtxParent;

	// ŽqƒIƒuƒWƒFƒNƒg‚ÉÄ‹A“I”½‰f
	if (_this->child)
		UpdateTransformMtx(_this->child, &_this->mtxWorld);
}
