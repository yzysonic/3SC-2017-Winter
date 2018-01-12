#pragma once

#include "Common.h"
#include "Transform.h"

struct Rigidbody
{
	Transform	*parent;	//�@�e�I�u�W�F�N�g�̃g�����X�t�H�[���ւ̃|�C���^
	Vector3		pos;		// ���W
	Vector3		vel;		// ���x
	Vector3		acc;		// �����x
	bool		bGravity;	// �d�͓K�p�t���O
};

void InitRigidbody(Rigidbody *_this, Transform *parent);
void UpdateRigidbody(Rigidbody *_this);
void SetGravity(Vector3 value);