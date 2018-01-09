#pragma once

#include "Direct3D.h"
#include "Transform.h"
#include "Texture.h"
#include "Color.h"

#define	NUM_VERTEX_2D	(4)	// ���_��
#define	NUM_POLYGON_2D	(2)	// �|���S����

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
struct Vertex2D
{
	Vector3 pos;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;		// �e�N�X�`�����W(*33)
};

struct RectPolygon2D
{
	Transform transform;
	Vertex2D vertex[NUM_VERTEX_2D];			// ���_
	Texture *pTexture;							// �e�N�X�`���ւ̃|�C���^
	Color color;							// �\������F
	float radius;							// ���_�v�Z�p���a
	float baseAngle;						// ���_�v�Z�p�p�x
	unsigned int pattern;					// �\������p�^�[��
};

void InitRectPolygon2D(RectPolygon2D *_this, Texture* tex);
void DrawRectPolygon2D(RectPolygon2D *_this);
void UninitRectPolygon2D(RectPolygon2D *_this);
void SetRectPolygon2DTexture(RectPolygon2D *_this, Texture* tex);
void SetRectPolygon2DSize(RectPolygon2D *_this, Vector2 size);
void SetRectPolygon2DPattern(RectPolygon2D *_this, UINT pattern);
void SetRectPolygon2DColor(RectPolygon2D *_this, Color color);
void SetRectPolygon2DOpacity(RectPolygon2D *_this, float opacity);