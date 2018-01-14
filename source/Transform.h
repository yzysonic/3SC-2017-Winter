#pragma once

#include <d3dx9math.h>
#include "Vector.h"

struct Transform
{
	Transform *child;
	Transform *sibling;
	Vector3 pos;
	Vector3 rot;
	Vector3 scl;
	D3DXMATRIX mtxLocal;
	D3DXMATRIX mtxWorld;
};

void InitTransform(Transform *_this);
void UpdateTransformMtx(Transform *_this, D3DXMATRIX *mtxParent = NULL);
void AddTransformChild(Transform *_this, Transform *child);
void AddTransformSibling(Transform *_this, Transform *sibling);