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

	// �X�V�O�̍��W���L�^����
	_this->pos_last = _this->pos;

	// ���͂̌v�Z
	_this->net_force = _this->force + _this->bGravity*g_gravity*_this->mass;

	// �����v�Z
	_this->pos += _this->vel*dt + 0.5f*_this->acc*dt*dt;	//�ʒu�̍X�V
	_this->vel += 0.5f*_this->acc*dt;	//���x�̍X�V(1)
	_this->acc = _this->net_force / _this->mass;	//�͂̌v�Z
	_this->vel += 0.5f*_this->acc*dt;	//���x�̍X�V(2)

	// �ړ��������v�Z
	_this->move_last = _this->pos - _this->pos_last;

	// �e�̍��W���X�V
	_this->parent->pos = _this->pos;

	// �͂̏�����
	_this->force = Vector3::zero;
	_this->net_force = _this->bGravity*g_gravity*_this->mass;
}

void SetGravity(Vector3 value)
{
	g_gravity = value;
}
