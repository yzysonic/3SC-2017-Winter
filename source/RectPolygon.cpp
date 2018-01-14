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

	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(
		sizeof(Vertex3D) * RECT_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&_this->pVtxBuff,			// 頂点バッファインターフェースへのポインタ
		NULL);
	
	Vertex3D *pVtx;

	_this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(ローカル座標)
	pVtx[0].pos = Vector3(-0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[1].pos = Vector3(0.5f*_this->size.x, 0.5f*_this->size.y, 0.0f);
	pVtx[2].pos = Vector3(-0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);
	pVtx[3].pos = Vector3(0.5f*_this->size.x, -0.5f*_this->size.y, 0.0f);

	// 反射光の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = Color::white;

	// 法線ベクトルの設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = Vector3(0.0f, 0.0f, -1.0f);

	// テクスチャ座標の設定
	pVtx[0].uv = Vector2(0.0f, 0.0f);
	pVtx[1].uv = Vector2(1.0f, 0.0f);
	pVtx[2].uv = Vector2(0.0f, 1.0f);
	pVtx[3].uv = Vector2(1.0f, 1.0f);

	_this->pVtxBuff->Unlock();

}

void DrawRectPolygon(RectPolygon * _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &_this->transform.mtxWorld);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, _this->pVtxBuff, 0, sizeof(Vertex3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	if(_this->pTexture)
		pDevice->SetTexture(0, _this->pTexture->pDXTex);
	else
		pDevice->SetTexture(0, NULL);
	// ポリゴンの描画
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

	// 頂点座標の設定(ローカル座標)
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
