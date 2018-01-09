//=============================================================================
//
// ���f������ [Model.cpp]
// Author : �k�q�B
//
//=============================================================================
#include "Model.h"
#include "Direct3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PATH		"../Data/MODEL/"	// �ǂݍ��ރ��f����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(Model* _this, const char* fileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char path[256];

	// ���f���֌W�̏�����
	_this->pBuffMaterial = NULL;
	_this->pMesh = NULL;
	_this->pTextures = NULL;
	_this->alphaTestEnable = false;

	InitTransform(&_this->transform);

	strcpy(path, MODEL_PATH);
	strcat(path, fileName);

	// X�t�@�C���̓ǂݍ���
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

	//�}�e���A���ƃe�N�X�`���̐ݒ�
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
// �I������
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
// �`�揈��
//=============================================================================
void DrawModel(Model* _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	for (DWORD i = 0; i < _this->numMaterial; i++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&_this->pMaterials[i].MatD3D);

		//�e�N�X�`���̐ݒ�
		if(_this->pTextures[i])
			pDevice->SetTexture(0, _this->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//�`��
		_this->pMesh->DrawSubset(i);
	}

	//�}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}