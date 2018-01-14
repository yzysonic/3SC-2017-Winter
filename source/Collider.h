#pragma once
#include "Vector.h"
#include "Model.h"
#include "RectPolygon.h"

struct PolyCollider
{
	Vector3 vtx[3];
	Vector3 normal;
	bool isTrigger;
	bool isCollidinig;
};

struct MeshCollider
{
	PolyCollider *polyList;
	int polyNum;
};

struct SphereCollider
{
	Vector3 pos;
	MeshCollider sphere;
	float radius;
	bool active;
};

void CalulatePolyColliderNormal(PolyCollider *_this);
void InitMeshColliderByModel(MeshCollider *_this, const Model *model);
void InitMeshColliderByPoly(MeshCollider *_this, const RectPolygon *poly);
void InitSphereCollider(SphereCollider *_this, Vector3 *pos);
void UninitMeshCollider(MeshCollider *_this);
void DrawPolyCollider(PolyCollider *_this);
void DrawMeshCollider(MeshCollider *_this);
void DrawSphereCollider(SphereCollider *_this);
void UnintSphereCollider(SphereCollider *_this);