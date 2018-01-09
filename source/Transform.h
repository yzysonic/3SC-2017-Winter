#pragma once

#include <d3dx9math.h>
#include "Vector.h"

struct Transform
{
	Transform *child;
	Vector3 pos;
	Vector3 rot;
	Vector3 scl;
	D3DXMATRIX mtxLocal;
	D3DXMATRIX mtxWorld;
};

void InitTransform(Transform *_this);
void UpdateTransformMtx(Transform *_this, D3DXMATRIX *mtxParent = NULL);