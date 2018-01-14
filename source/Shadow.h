//=============================================================================
//
// �V���h�E���� [shadow.h]
// Author : GP11A243 �k �q�B
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "core.h"

struct Shadow
{
	Transform transform;
	Vector3 *parent_pos = NULL;		// �����僂�f��
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitShadow(Shadow *_this, Transform *parent);
void UninitShadow(Shadow *_this);
void UpdateShadow(Shadow *_this);
void DrawShadow(void);

#endif