#pragma once

#include "Common.h"
#include "Collider.h"
#include "Rigidbody.h"

struct Collision
{
	Vector3 point;
	Vector3 normal;
	Vector3 offset;
};

bool CollisionTestPolyAndLine(PolyCollider &collider, const Vector3 &linePos, const Vector3 &lineVec, Collision* pOutCollision = NULL);
bool CollisionTestMeshAndLine(MeshCollider &collider, const Vector3 &linePos, const Vector3 &lineVec, Collision* pOutCollision = NULL);
bool CollisionTestSphere(SphereCollider &collider_a, SphereCollider &collider_b, Collision* pOutCollision = NULL);
void CollisionResponse(const Collision *collision, Rigidbody *rb);