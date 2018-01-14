//=============================================================================
//
// モデル処理 [AnimatedModel.cpp]
// Author : 楊子毅
//
//=============================================================================
#include "AnimatedModel.h"
#include "Texture.h"
#include "Direct3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PATH		"../Data/MODEL/"	// 読み込むモデル名

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAnimatedModel(AnimatedModel* _this, const char* fileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AllocateHierarchy	allocater;
	char path[256];


	// モデル関係の初期化
	_this->rootFrame = NULL;
	_this->meshCont = NULL;
	_this->animationSet = NULL;
	//_this->activeAnimation = 0;
	_this->alphaTestEnable = false;

	InitTransform(&_this->transform);

	strcpy(path, MODEL_PATH);
	strcat(path, fileName);

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		path,
		D3DXMESH_SYSTEMMEM, //D3DXMESH_MANAGED
		pDevice,
		&allocater,
		0,
		&_this->rootFrame,
		&_this->animator)))
	{
		return E_FAIL;
	}

	_this->meshCont = FindMeshContainer(_this->rootFrame);

	//マテリアルの設定
	for (int i = 0; i < (int)_this->meshCont->NumMaterials; i++)
		_this->meshCont->pMaterials[i].MatD3D.Ambient = _this->meshCont->pMaterials[i].MatD3D.Diffuse;

	// アニメーターの設定
	if (_this->animator)
	{
		if (_this->animator->GetMaxNumAnimationSets() > 1)
		{
			int n = _this->animator->GetMaxNumAnimationSets();
			_this->animationSet = (LPD3DXANIMATIONSET*)malloc(sizeof(LPD3DXANIMATIONSET)*n);
			for (int i = 0; i < n; i++)
				_this->animator->GetAnimationSet(i, &_this->animationSet[i]);
		}
	}

	// ワールドマトリクスの初期化
	_this->transform.mtxLocal = _this->rootFrame->TransformationMatrix * _this->rootFrame->pFrameFirstChild->TransformationMatrix;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAnimatedModel(AnimatedModel* _this)
{
	AllocateHierarchy	allocater;

	SafeFree(_this->animationSet);
	allocater.DestroyFrame(_this->rootFrame);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawAnimatedModel(AnimatedModel* _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// アニメーション更新
	_this->animator->AdvanceTime(0.016f, 0);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	for (DWORD i = 0; i < _this->meshCont->NumMaterials; i++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&_this->meshCont->pMaterials[i].MatD3D);

		//テクスチャの設定
		if (_this->meshCont->pTextures[i])
			pDevice->SetTexture(0, _this->meshCont->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//描画
		_this->meshCont->MeshData.pMesh->DrawSubset(i);
	}

	//マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
