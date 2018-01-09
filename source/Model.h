//=============================================================================
//
// モデル処理 [Model.h]
// Author : 楊子毅
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "Common.h"
#include "Transform.h"
#include "Texture.h"

struct Model
{
	Transform			transform;		// トランスフォーム
	Texture				**pTextures;	// テクスチャへのポインタ
	LPD3DXMESH			pMesh;			// ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER		pBuffMaterial;	// マテリアルバッファーへのポインタ
	LPD3DXMATERIAL		pMaterials;		// マテリアル情報へのポインタ
	DWORD				numMaterial;	// マテリアル情報の数
	bool				alphaTestEnable;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitModel(Model* _this, const char* fileName);
void UninitModel(Model* _this);
void DrawModel(Model* _this);

#endif
