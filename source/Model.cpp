//=============================================================================
//
// モデル処理 [Model.cpp]
// Author : 楊子毅
//
//=============================================================================
#include "Model.h"
#include "Direct3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PATH		"../Data/MODEL/"	// 読み込むモデル名

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(Model* _this, const char* fileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char path[256];

	// モデル関係の初期化
	_this->pBuffMaterial = NULL;
	_this->pMesh = NULL;
	_this->pTextures = NULL;
	_this->alphaTestEnable = false;

	InitTransform(&_this->transform);

	strcpy(path, MODEL_PATH);
	strcat(path, fileName);

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(path,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&_this->pBuffMaterial,
		NULL,
		&_this->numMaterial,
		&_this->pMesh)))
	{
		return E_FAIL;
	}

	//マテリアルとテクスチャの設定
	_this->pMaterials = (LPD3DXMATERIAL)_this->pBuffMaterial->GetBufferPointer();
	_this->pTextures = (Texture**)malloc(sizeof(Texture*)*_this->numMaterial);

	for (DWORD i = 0; i < _this->numMaterial; i++)
	{
		_this->pMaterials[i].MatD3D.Ambient = _this->pMaterials[i].MatD3D.Diffuse;

		char* texFile = _this->pMaterials[i].pTextureFilename;
		if (texFile)
		{
			char texName[256];
			strcpy(texName, texFile);
			strtok(texName, ".");
			_this->pTextures[i] = LoadTexture(texName, _this->pMaterials[i].pTextureFilename);
		}
		else
			_this->pTextures[i] = NULL;
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(Model* _this)
{
	SafeRelease(_this->pMesh);
	SafeRelease(_this->pBuffMaterial);
	for (DWORD i = 0; i < _this->numMaterial; i++)
		ReleaseTexture(&_this->pTextures[i]);
	SafeFree(_this->pTextures);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(Model* _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	for (DWORD i = 0; i < _this->numMaterial; i++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&_this->pMaterials[i].MatD3D);

		//テクスチャの設定
		if(_this->pTextures[i])
			pDevice->SetTexture(0, _this->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//描画
		_this->pMesh->DrawSubset(i);
	}

	//マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}