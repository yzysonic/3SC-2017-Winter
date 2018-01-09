//=============================================================================
//
// ���f������ [SkinnedModel.h]
// Author : �k�q�B
//
//=============================================================================
#ifndef _SKINNED_MODEL_H_
#define _SKINNED_MODEL_H_

#include "Common.h"
#include "AllocateHirerachy.h"
#include "Bone.h"
#include "Transform.h"

struct SkinnedModel
{
	D3DXFRAME			*rootFrame;
	XMESHCONTAINER		*meshCont;
	D3DXBONECOMBINATION *combs;
	Bone				**boneMap;
	ID3DXAnimationController *animator;
	LPD3DXANIMATIONSET	*animationSet;
	Transform			transform;

	int					activeAnimation;
	bool				alphaTestEnable;
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSkinnedModel(SkinnedModel* _this, const char* fileName);
void UninitSkinnedModel(SkinnedModel* _this);
void DrawSkinnedModel(SkinnedModel* _this);

#endif
