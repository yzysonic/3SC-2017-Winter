#include "Transform.h"

void InitTransform(Transform * _this)
{
	_this->child = NULL;
	_this->sibling = NULL;
	_this->pos = Vector3(0.0f, 0.0f, 0.0f);
	_this->rot = Vector3(0.0f, 0.0f, 0.0f);
	_this->scl = Vector3(1.0f, 1.0f, 1.0f);
	D3DXMatrixIdentity(&_this->mtxLocal);
	D3DXMatrixIdentity(&_this->mtxWorld);
}

void UpdateTransformMtx(Transform * _this, D3DXMATRIX *mtxParent)
{
	D3DXMATRIX temp;

	// ���[�J����Ԃ̕ϊ��𔽉f
	_this->mtxWorld = _this->mtxLocal;

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&temp, _this->scl.x, _this->scl.y, _this->scl.z);
	_this->mtxWorld *= temp;

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&temp, _this->rot.y, _this->rot.x, _this->rot.z);
	_this->mtxWorld *= temp;

	// �ړ��𔽉f
	D3DXMatrixTranslation(&temp, _this->pos.x, _this->pos.y, _this->pos.z);
	_this->mtxWorld *= temp;

	// �e��Ԃ̕ϊ��𔽉f
	if (mtxParent)
		_this->mtxWorld *= *mtxParent;

	// �q�I�u�W�F�N�g�ɔ��f
	if (_this->child)
		UpdateTransformMtx(_this->child, &_this->mtxWorld);
	// �Z��I�u�W�F�N�g�ɔ��f
	if (_this->sibling)
		UpdateTransformMtx(_this->sibling, mtxParent);
}

void AddTransformChild(Transform * _this, Transform * child)
{
	if (_this->child == NULL)
	{
		_this->child = child;
	}
	else
	{
		AddTransformSibling(_this->child, child);
	}
}

void AddTransformSibling(Transform * _this, Transform * sibling)
{
	if (_this->sibling == NULL)
	{
		_this->sibling = sibling;
	}
	else
	{
		AddTransformSibling(_this->sibling, sibling);
	}
}
