#include "Rigidbody.h"
#include "Time.h"

Vector3 g_gravity;

void InitRigidbody(Rigidbody * _this, Transform * parent)
{
	_this->parent = parent;
	_this->pos = parent->pos;
	_this->mass = 1.0f;
	_this->bounciness = 0.0f;
	_this->bGravity = false;
}

void UpdateRigidbody(Rigidbody * _this)
{
	float dt = DeltaTime();

	// 更新前の座標を記録する
	_this->pos_last = _this->pos;

	// 合力の計算
	_this->net_force = _this->force + _this->bGravity*g_gravity*_this->mass;

	// 物理計算
	_this->pos += _this->vel*dt + 0.5f*_this->acc*dt*dt;	//位置の更新
	_this->vel += 0.5f*_this->acc*dt;	//速度の更新(1)
	_this->acc = _this->net_force / _this->mass;	//力の計算
	_this->vel += 0.5f*_this->acc*dt;	//速度の更新(2)

	// 移動履歴を計算
	_this->move_last = _this->pos - _this->pos_last;

	// 親の座標を更新
	_this->parent->pos = _this->pos;

	// 力の初期化
	_this->force = Vector3::zero;
	_this->net_force = _this->bGravity*g_gravity*_this->mass;
}

void SetGravity(Vector3 value)
{
	g_gravity = value;
}
