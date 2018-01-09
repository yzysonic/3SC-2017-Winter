#pragma once

#include "Direct3D.h"
#include "Transform.h"
#include "Texture.h"
#include "Color.h"

#define	NUM_VERTEX_2D	(4)	// 頂点数
#define	NUM_POLYGON_2D	(2)	// ポリゴン数

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 上記頂点フォーマットに合わせた構造体を定義
struct Vertex2D
{
	Vector3 pos;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
	Vector2 uv;		// テクスチャ座標(*33)
};

struct RectPolygon2D
{
	Transform transform;
	Vertex2D vertex[NUM_VERTEX_2D];			// 頂点
	Texture *pTexture;							// テクスチャへのポインタ
	Color color;							// 表示する色
	float radius;							// 頂点計算用半径
	float baseAngle;						// 頂点計算用角度
	unsigned int pattern;					// 表示するパターン
};

void InitRectPolygon2D(RectPolygon2D *_this, Texture* tex);
void DrawRectPolygon2D(RectPolygon2D *_this);
void UninitRectPolygon2D(RectPolygon2D *_this);
void SetRectPolygon2DTexture(RectPolygon2D *_this, Texture* tex);
void SetRectPolygon2DSize(RectPolygon2D *_this, Vector2 size);
void SetRectPolygon2DPattern(RectPolygon2D *_this, UINT pattern);
void SetRectPolygon2DColor(RectPolygon2D *_this, Color color);
void SetRectPolygon2DOpacity(RectPolygon2D *_this, float opacity);