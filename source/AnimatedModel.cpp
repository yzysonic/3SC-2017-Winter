//=============================================================================
//
// ���f������ [AnimatedModel.cpp]
// Author : �k�q�B
//
//=============================================================================
#include "AnimatedModel.h"
#include "Texture.h"
#include "Direct3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PATH		"../Data/MODEL/"	// �ǂݍ��ރ��f����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAnimatedModel(AnimatedModel* _this, const char* fileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AllocateHierarchy	allocater;
	char path[256];


	// ���f���֌W�̏�����
	_this->rootFrame = NULL;
	_this->meshCont = NULL;
	_this->animationSet = NULL;
	//_this->activeAnimation = 0;
	_this->alphaTestEnable = false;

	InitTransform(&_this->transform);

	strcpy(path, MODEL_PATH);
	strcat(path, fileName);

	// X�t�@�C���̓ǂݍ���
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

	//�}�e���A���̐ݒ�
	for (int i = 0; i < (int)_this->meshCont->NumMaterials; i++)
		_this->meshCont->pMaterials[i].MatD3D.Ambient = _this->meshCont->pMaterials[i].MatD3D.Diffuse;

	// �A�j���[�^�[�̐ݒ�
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

	// ���[���h�}�g���N�X�̏�����
	_this->transform.mtxLocal = _this->rootFrame->TransformationMatrix * _this->rootFrame->pFrameFirstChild->TransformationMatrix;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAnimatedModel(AnimatedModel* _this)
{
	AllocateHierarchy	allocater;

	SafeFree(_this->animationSet);
	allocater.DestroyFrame(_this->rootFrame);
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawAnimatedModel(AnimatedModel* _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// �A�j���[�V�����X�V
	_this->animator->AdvanceTime(0.016f, 0);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	for (DWORD i = 0; i < _this->meshCont->NumMaterials; i++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&_this->meshCont->pMaterials[i].MatD3D);

		//�e�N�X�`���̐ݒ�
		if (_this->meshCont->pTextures[i])
			pDevice->SetTexture(0, _this->meshCont->pTextures[i]->pDXTex);
		else
			pDevice->SetTexture(0, NULL);

		//�`��
		_this->meshCont->MeshData.pMesh->DrawSubset(i);
	}

	//�}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
