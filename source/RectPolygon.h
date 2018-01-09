#pragma once

#include "Direct3D.h"
#include "Transform.h"
#include "Texture.h"
#include "Color.h"

#define	RECT_NUM_VERTEX		(4)		// 頂点数
#define	RECT_NUM_POLYGON	(2)		// ポリゴン数

// 3Dポリゴン頂点フォーマット( 頂点座標 / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct Vertex3D
{
	Vector3 pos;
	Vector3 nor;
	Color diffuse;
	Vector2 uv;
};

struct RectPolygon
{
	Transform transform;
	Vertex3D vertex[RECT_NUM_VERTEX];
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	Texture *pTexture;
	Color color;
	Vector2 size;
};

void InitRectPolygon(RectPolygon *_this, Texture* tex);
void DrawRectPolygon(RectPolygon *_this);
void UninitRectPolygon(RectPolygon *_this);
void SetRectPolygonTexture(RectPolygon *_this, Texture* tex);
void SetRectPolygonSize(RectPolygon *_this, Vector2 size);
void SetRectPolygonColor(RectPolygon *_this, Color color);
void SetRectPolygonOpacity(RectPolygon *_this, float opacity);