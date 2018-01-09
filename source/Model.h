//=============================================================================
//
// ���f������ [Model.h]
// Author : �k�q�B
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "Common.h"
#include "Transform.h"
#include "Texture.h"

struct Model
{
	Transform			transform;		// �g�����X�t�H�[��
	Texture				**pTextures;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pMesh;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
	LPD3DXBUFFER		pBuffMaterial;	// �}�e���A���o�b�t�@�[�ւ̃|�C���^
	LPD3DXMATERIAL		pMaterials;		// �}�e���A�����ւ̃|�C���^
	DWORD				numMaterial;	// �}�e���A�����̐�
	bool				alphaTestEnable;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitModel(Model* _this, const char* fileName);
void UninitModel(Model* _this);
void DrawModel(Model* _this);

#endif
