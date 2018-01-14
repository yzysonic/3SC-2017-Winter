#include "RectPolygon.h"
#include "Common.h"

void InitRectPolygon(RectPolygon * _this, Texture * tex)
{
	InitTransform(&_this->transform);

	_this->pTexture = tex;

	if (tex)
		_this->size = tex->size;
	else
		_this->size = Vector2::one*10.0f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(
		sizeof(Vertex3D) * RECT_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&_this->pVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);
	
	Vertex3D *pVtx;

	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���[�J�����W)
	pVtx[0].pos = Vector3(-0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[1].pos = Vector3(0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[2].pos = Vector3(-0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);
	pVtx[3].pos = Vector3(0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = Color::white;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = Vector3(0.0f, 0.0f, -1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].uv = Vector2(0.0f, 0.0f);
	pVtx[1].uv = Vector2(1.0f, 0.0f);
	pVtx[2].uv = Vector2(0.0f, 1.0f);
	pVtx[3].uv = Vector2(1.0f, 1.0f);

	_this->pVtxBuff->Unlock();

}

void DrawRectPolygon(RectPolygon * _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, _this->pVtxBuff, 0, sizeof(Vertex3D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	if(_this->pTexture)
		pDevice->SetTexture(0, _this->pTexture->pDXTex);
	else
		pDevice->SetTexture(0, NULL);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);

}

void UninitRectPolygon(RectPolygon * _this)
{
	SafeRelease(_this->pVtxBuff);
}

void SetRectPolygonTexture(RectPolygon * _this, Texture * tex)
{
	_this->pTexture = tex;

	if (tex)
		SetRectPolygonSize(_this, tex->size);
	else
		SetRectPolygonSize(_this, Vector2::one*10.0f);
}

void SetRectPolygonSize(RectPolygon * _this, Vector2 size)
{
	_this->size = size;

	Vertex3D *pVtx;

	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���[�J�����W)
	pVtx[0].pos = Vector3(-0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[1].pos = Vector3(0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[2].pos = Vector3(-0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);
	pVtx[3].pos = Vector3(0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);

	_this->pVtxBuff->Unlock();

}

void SetRectPolygonColor(RectPolygon * _this, Color color)
{
	Vertex3D *pVtx;

	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = color;

	_this->pVtxBuff->Unlock();

	_this->color = color;
}

void SetRectPolygonOpacity(RectPolygon * _this, float opacity)
{
	Vertex3D *pVtx;

	_this->color.a = (UCHAR)(255 * opacity);

	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse.a =
	pVtx[1].diffuse.a =
	pVtx[2].diffuse.a =
	pVtx[3].diffuse.a = _this->color.a;

	_this->pVtxBuff->Unlock();

}

Vertex3D *LockRectPolygonVtx(RectPolygon * _this)
{
	Vertex3D *pVtx;
	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	return pVtx;
}

void UnlockRectPolygonVtx(const RectPolygon * _this)
{
	_this->pVtxBuff->Unlock();
}
