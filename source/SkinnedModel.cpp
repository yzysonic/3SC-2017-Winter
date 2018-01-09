//=============================================================================
//
// ���f������ [SkinnedModel.cpp]
// Author : �k�q�B
//
//=============================================================================
#include "SkinnedModel.h"
#include "Texture.h"
#include "Direct3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PATH		"../Data/MODEL/"	// �ǂݍ��ރ��f����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
XMESHCONTAINER *FindMeshContainer(D3DXFRAME *frame);
D3DXFRAME *FindFrameByName(const char* name, D3DXFRAME *frame);


//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkinnedModel(SkinnedModel* _this, const char* fileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AllocateHierarchy	allocater;
	char path[256];


	// ���f���֌W�̏�����
	_this->rootFrame = NULL;
	_this->meshCont = NULL;
	_this->combs = NULL;
	_this->boneMap = NULL;
	_this->animationSet = NULL;
	_this->activeAnimation = 0;
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

	// �{�[���ݒ�
	if (_this->meshCont->pSkinInfo != NULL)
	{
		_this->combs = (D3DXBONECOMBINATION*)_this->meshCont->boneCombinationTable->GetBufferPointer();
		_this->boneMap = (Bone**)malloc(sizeof(Bone*)*_this->meshCont->pSkinInfo->GetNumBones());
		for (DWORD i = 0; i < _this->meshCont->pSkinInfo->GetNumBones(); i++)
		{
			_this->boneMap[i] = (Bone*)FindFrameByName(_this->meshCont->pSkinInfo->GetBoneName(i), _this->rootFrame);
			_this->boneMap[i]->mtxOffset = *_this->meshCont->pSkinInfo->GetBoneOffsetMatrix(i);
		}
	}

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
void UninitSkinnedModel(SkinnedModel* _this)
{
	AllocateHierarchy	allocater;

	SafeFree(_this->boneMap);
	SafeFree(_this->animationSet);

	allocater.DestroyFrame(_this->rootFrame);
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkinnedModel(SkinnedModel* _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _this->alphaTestEnable);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (_this->meshCont->pSkinInfo != NULL)
	{
		// �A�j���[�V�����X�V
		_this->animator->AdvanceTime(0.016f, 0);
		// �{�[���}�g���N�X�X�V
		UpdateBoneMatrix(_this->transform.mtxWorld, (Bone*)_this->rootFrame->pFrameFirstChild->pFrameFirstChild);

		for (DWORD i = 0; i < _this->meshCont->numBoneCombinations; i++) {
			DWORD boneNum = 0;
			for (DWORD j = 0; j < _this->meshCont->maxFaceInfl; j++) {
				DWORD id = _this->combs[i].BoneId[j];
				if (id != UINT_MAX) {
					pDevice->SetTransform(D3DTS_WORLDMATRIX(j), &(_this->boneMap[id]->mtxCombined));
					boneNum++;
				}
			}
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, boneNum - 1);

			int attribID = _this->combs[i].AttribId;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&_this->meshCont->pMaterials[attribID].MatD3D);

			//�e�N�X�`���̐ݒ�
			if(_this->meshCont->pTextures[attribID])
				pDevice->SetTexture(0, _this->meshCont->pTextures[attribID]->pDXTex);
			else
				pDevice->SetTexture(0, NULL);
			//�`��
			_this->meshCont->MeshData.pMesh->DrawSubset(i);
		}
	}

	else
	{
		// ���[���h�}�g���N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

		for (DWORD i = 0; i < _this->meshCont->NumMaterials; i++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&_this->meshCont->pMaterials[i].MatD3D);

			//�e�N�X�`���̐ݒ�
			if(_this->meshCont->pTextures[i])
				pDevice->SetTexture(0, _this->meshCont->pTextures[i]->pDXTex);
			else
				pDevice->SetTexture(0, NULL);

			//�`��
			_this->meshCont->MeshData.pMesh->DrawSubset(i);
		}
	}



	 //�}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}


XMESHCONTAINER * FindMeshContainer(D3DXFRAME * frame)
{
	if (frame->pMeshContainer)
		return (XMESHCONTAINER*)frame->pMeshContainer;

	XMESHCONTAINER* temp = NULL;

	if (frame->pFrameSibling)
	{
		temp = FindMeshContainer(frame->pFrameSibling);
		if (temp) return temp;
	}

	if (frame->pFrameFirstChild)
	{
		temp = FindMeshContainer(frame->pFrameFirstChild);
		if (temp) return temp;
	}


	return NULL;
}

D3DXFRAME * FindFrameByName(const char * name, D3DXFRAME *frame)
{
	D3DXFRAME *ptr = frame;

	if (strcmp(ptr->Name, name) == 0)
		return ptr;
	if (frame->pFrameFirstChild)
	{
		ptr = FindFrameByName(name, frame->pFrameFirstChild);
		if (ptr) return ptr;
	}
	if (frame->pFrameSibling)
	{
		ptr = FindFrameByName(name, frame->pFrameSibling);
		if (ptr) return ptr;
	}
	return NULL;
	
}