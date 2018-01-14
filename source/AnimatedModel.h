//=============================================================================
//
// ÉÇÉfÉãèàóù [AnimatedModel.h]
// Author : ókéqãB
//
//=============================================================================
#ifndef _ANIMATED_MODEL_H_
#define _ANIMATED_MODEL_H_

#include "Common.h"
#include "AllocateHirerachy.h"
#include "Transform.h"

struct AnimatedModel
{
	D3DXFRAME			*rootFrame;
	XMESHCONTAINER		*meshCont;
	
	ID3DXAnimationController *animator;
	LPD3DXANIMATIONSET	*animationSet;
	Transform			transform;
	bool				alphaTestEnable;

};

HRESULT InitAnimatedModel(AnimatedModel* _this, const char* fileName);
void UninitAnimatedModel(AnimatedModel* _this);
void DrawAnimatedModel(AnimatedModel* _this);

#endif