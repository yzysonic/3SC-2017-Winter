#include "Rigidbody.h"
#include "Time.h"

Vector3 g_gravity;

void InitRigidbody(Rigidbody * _this, Transform * parent)
{
	_this->parent = parent;
	_this->pos = parent->pos;
	_this->bGravity = false;
}

void UpdateRigidbody(Rigidbody * _this)
{
	float dt = DeltaTime();
	Vector3 a = _this->acc + _this->bGravity*g_gravity;

	_this->pos += _this->vel*dt + 0.5f*a*dt*dt;			//�ʒu�̍X�V
	_this->vel += 0.5f*a*dt;	//���x�̍X�V(1)
								//�͂̌v�Z
	_this->vel += 0.5f*a*dt;	//���x�̍X�V(2)

	_this->parent->pos = _this->pos;
}

void SetGravity(Vector3 value)
{
	g_gravity = value;
}
