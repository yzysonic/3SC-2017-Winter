#pragma once

#include "Common.h"
#include "Transform.h"

struct Rigidbody
{
	Transform	*parent;	//　親オブジェクトのトランスフォームへのポインタ
	Vector3		pos;		// 座標
	Vector3		vel;		// 速度
	Vector3		acc;		// 加速度
	bool		bGravity;	// 重力適用フラグ
};

void InitRigidbody(Rigidbody *_this, Transform *parent);
void UpdateRigidbody(Rigidbody *_this);
void SetGravity(Vector3 value);