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

	// ローカル空間の変換を反映
	_this->mtxWorld = _this->mtxLocal;

	// スケールを反映
	D3DXMatrixScaling(&temp, _this->scl.x, _this->scl.y, _this->scl.z);
	_this->mtxWorld *= temp;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&temp, _this->rot.y, _this->rot.x, _this->rot.z);
	_this->mtxWorld *= temp;

	// 移動を反映
	D3DXMatrixTranslation(&temp, _this->pos.x, _this->pos.y, _this->pos.z);
	_this->mtxWorld *= temp;

	// 親空間の変換を反映
	if (mtxParent)
		_this->mtxWorld *= *mtxParent;

	// 子オブジェクトに反映
	if (_this->child)
		UpdateTransformMtx(_this->child, &_this->mtxWorld);
	// 兄弟オブジェクトに反映
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
