//=============================================================================
//
// シャドウ処理 [shadow.h]
// Author : GP11A243 楊 子毅
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "core.h"

struct Shadow
{
	Transform transform;
	Vector3 *parent_pos = NULL;		// 持ち主モデル
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(Shadow *_this, Transform *parent);
void UninitShadow(Shadow *_this);
void UpdateShadow(Shadow *_this);
void DrawShadow(void);

#endif