#include "RectPolygon2D.h"

void InitRectPolygon2D(RectPolygon2D * _this, Texture* tex = NULL)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InitTransform(&_this->transform);
	SetRectPolygon2DTexture(_this, tex);
	SetRectPolygon2DPattern(_this, 0);
	SetRectPolygon2DColor(_this, Color::white);

	// rhwの設定
	_this->vertex[0].rhw =
	_this->vertex[1].rhw =
	_this->vertex[2].rhw =
	_this->vertex[3].rhw = 1.0f;

}

void DrawRectPolygon2D(RectPolygon2D * _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ワールド変換
	Vector3 &pos = _this->transform.pos;
	Vector3 &rot = _this->transform.rot;
	Vector3 radius = _this->radius * _this->transform.scl;

	_this->vertex[0].pos.x = pos.x - cosf(_this->baseAngle + rot.z) * radius.x;
	_this->vertex[0].pos.y = pos.y - sinf(_this->baseAngle + rot.z) * radius.y;
	_this->vertex[0].pos.z = pos.z;

	_this->vertex[1].pos.x = pos.x + cosf(_this->baseAngle - rot.z) * radius.x;
	_this->vertex[1].pos.y = pos.y - sinf(_this->baseAngle - rot.z) * radius.y;
	_this->vertex[1].pos.z = pos.z;

	_this->vertex[2].pos.x = pos.x - cosf(_this->baseAngle - rot.z) * radius.x;
	_this->vertex[2].pos.y = pos.y + sinf(_this->baseAngle - rot.z) * radius.y;
	_this->vertex[2].pos.z = pos.z;

	_this->vertex[3].pos.x = pos.x + cosf(_this->baseAngle + rot.z) * radius.x;
	_this->vertex[3].pos.y = pos.y + sinf(_this->baseAngle + rot.z) * radius.y;
	_this->vertex[3].pos.z = pos.z;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, _this->pTexture->pDXTex);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON_2D, _this->vertex, sizeof(Vertex2D));

}

void UninitRectPolygon2D(RectPolygon2D * _this)
{
}

void SetRectPolygon2DTexture(RectPolygon2D * _this, Texture * tex)
{
	if (tex)
		SetRectPolygon2DSize(_this, tex->size);
	else
		SetRectPolygon2DSize(_this, Vector2(10.f, 10.f));

	_this->pTexture = tex;
}

void SetRectPolygon2DSize(RectPolygon2D * _this, Vector2 size)
{
	_this->baseAngle = atan2f(size.y, size.x);
	_this->radius = size.length()/2.0f;
}

void SetRectPolygon2DPattern(RectPolygon2D * _this, UINT pattern)
{
	_this->pattern = pattern;

	int x = pattern % _this->pTexture->divideX;
	int y = pattern / _this->pTexture->divideX;
	Vector2 size = Vector2(1.0f / _this->pTexture->divideX, 1.0f / _this->pTexture->divideY);

	_this->vertex[0].uv = Vector2(x*size.x, y*size.y);
	_this->vertex[1].uv = Vector2(x*size.x + size.x, y*size.y);
	_this->vertex[2].uv = Vector2(x*size.x, y*size.y + size.y);
	_this->vertex[3].uv = Vector2(x*size.x + size.x, y*size.y + size.y);

}

void SetRectPolygon2DColor(RectPolygon2D * _this, Color color)
{
	_this->color = color;
	_this->vertex[0].diffuse =
		_this->vertex[1].diffuse =
		_this->vertex[2].diffuse =
		_this->vertex[3].diffuse = color;

}

void SetRectPolygon2DOpacity(RectPolygon2D * _this, float opacity)
{
	_this->color.a = (UCHAR)(opacity*255);
	_this->vertex[0].diffuse.a =
	_this->vertex[1].diffuse.a =
	_this->vertex[2].diffuse.a =
	_this->vertex[3].diffuse.a = _this->color.a;
}
