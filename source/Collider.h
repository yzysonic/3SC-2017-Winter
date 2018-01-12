#pragma once
#include "Vector.h"
#include "Model.h"

struct PolyCollider
{
	Vector3 vtx[3];
	Vector3 normal;
	bool isCollidinig;
};

struct MeshCollider
{
	PolyCollider *polyList;
	int polyNum;
};

void CalulatePolyColliderNormal(PolyCollider *_this);
void InitMeshColliderByModel(MeshCollider *_this, const Model *model);
void UninitMeshCollider(MeshCollider *_this);
void DrawPolyCollider(PolyCollider *_this);
void DrawMeshCollider(MeshCollider *_this);