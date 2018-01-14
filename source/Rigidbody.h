#pragma once

#include "Common.h"
#include "Transform.h"

struct Rigidbody
{
	Transform	*parent;	//　親オブジェクトのトランスフォームへのポインタ
	Vector3		pos;		// 座標
	Vector3		vel;		// 速度
	Vector3		acc;		// 加速度
	Vector3		force;		// 力
	Vector3		net_force;	// 合力
	Vector3		pos_last;	// 更新前の座標
	Vector3		move_last;	// 移動履歴のベクトル
	float		mass;		// 質量
	float		bounciness; // 弾性係数
	bool		bGravity;	// 重力適用フラグ
};

void InitRigidbody(Rigidbody *_this, Transform *parent);
void UpdateRigidbody(Rigidbody *_this);
void SetGravity(Vector3 value);