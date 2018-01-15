//=============================================================================
//
// �V���h�E���� [shadow.cpp]
// Author : GP11A243 �k �q�B
//
//=============================================================================
#include "Shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW	"shadow"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define SHADOW_MAX	(20)

#define	SHADOW_SIZE_X	(10.0f)			// �e�̃T�C�Y(X����)
#define	SHADOW_SIZE_Z	(10.0f)			// �e�̃T�C�Y(Z����)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture	*g_pTextureShadow = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxVuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Shadow *g_shadowList[SHADOW_MAX];
int g_shadowNum = 0;


//=============================================================================
// ����������
//=============================================================================
void InitShadow(Shadow* _this, Transform *parent)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_shadowNum == 0)
	{
		// ���_�A���̍쐬
		MakeVertexShadow();
		// �e�N�X�`���̓ǂݍ���
		g_pTextureShadow = GetTexture(TEXTURE_SHADOW);
	}

	// �ʒu�E��]�E�X�P�[���̐ݒ�
	InitTransform(&_this->transform);

	AddTransformChild(parent, &_this->transform);
	if (g_shadowNum < SHADOW_MAX)
		g_shadowList[g_shadowNum] = _this;

	g_shadowNum++;

}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(Shadow* _this)
{
	for (int i = 0; i < g_shadowNum; i++)
	{
		if (g_shadowList[i] == _this)
		{
			g_shadowList[i] = g_shadowList[g_shadowNum - 1];
			g_shadowList[g_shadowNum - 1] = NULL;
			break;
		}
	}

	g_shadowNum--;

	if (g_shadowNum == 0)
	{
		SafeRelease(g_pVtxVuffShadow);
		ReleaseTexture(g_pTextureShadow->file_name);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
	//for (int i = 0; i < g_shadowNum; i++)
	//{
	//	g_shadowList[i].scl.x = g_shadow[i].base_scl.x + 0.001f*g_shadow[i].parent_pos->y;
	//	g_shadowList[i].scl.z = g_shadow[i].base_scl.z + 0.001f*g_shadow[i].parent_pos->y;
	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���Z����
	DWORD blend_src, blend_dest;
	pDevice->GetRenderState(D3DRS_SRCBLEND, &blend_src);
	pDevice->GetRenderState(D3DRS_DESTBLEND, &blend_dest);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//DWORD blend_src, blend_dest;
	//pDevice->GetRenderState(D3DRS_SRCBLEND, &blend_src);
	//pDevice->GetRenderState(D3DRS_DESTBLEND, &blend_dest);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���C�g�Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxVuffShadow, 0, sizeof(Vertex3D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureShadow->pDXTex);

	for (int i = 0; i < g_shadowNum; i++)
	{
		// ���[���h�}�g���N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_shadowList[i]->transform.mtxWorld);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, blend_src);
	pDevice->SetRenderState(D3DRS_DESTBLEND, blend_dest);

	//pDevice->SetRenderState(D3DRS_SRCBLEND, blend_src);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, blend_dest);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexShadow(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxVuffShadow,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		Vertex3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxVuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�(���[�J�����W)
		pVtx[0].pos = Vector3(-SHADOW_SIZE_X, 0.0f, SHADOW_SIZE_Z);
		pVtx[1].pos = Vector3(SHADOW_SIZE_X, 0.0f, SHADOW_SIZE_Z);
		pVtx[2].pos = Vector3(-SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z);
		pVtx[3].pos = Vector3(SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = Vector3(-1.0f, 1.0f, 0.0f);
		pVtx[1].nor = Vector3(1.0f, 1.0f, 0.0f);
		pVtx[2].nor = Vector3(-1.0f, 1.0f, 0.0f);
		pVtx[3].nor = Vector3(1.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = Color(255, 255, 255, 60);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].uv = Vector2(0.0f, 0.0f);
		pVtx[1].uv = Vector2(1.0f, 0.0f);
		pVtx[2].uv = Vector2(0.0f, 1.0f);
		pVtx[3].uv = Vector2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxVuffShadow->Unlock();
	}

	return S_OK;
}
