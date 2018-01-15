//=============================================================================
//
// シャドウ処理 [shadow.cpp]
// Author : GP11A243 楊 子毅
//
//=============================================================================
#include "Shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW	"shadow"		// 読み込むテクスチャファイル名
#define SHADOW_MAX	(20)

#define	SHADOW_SIZE_X	(10.0f)			// 影のサイズ(X方向)
#define	SHADOW_SIZE_Z	(10.0f)			// 影のサイズ(Z方向)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShadow(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture	*g_pTextureShadow = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxVuffShadow = NULL;	// 頂点バッファへのポインタ
Shadow *g_shadowList[SHADOW_MAX];
int g_shadowNum = 0;


//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(Shadow* _this, Transform *parent)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_shadowNum == 0)
	{
		// 頂点、情報の作成
		MakeVertexShadow();
		// テクスチャの読み込み
		g_pTextureShadow = GetTexture(TEXTURE_SHADOW);
	}

	// 位置・回転・スケールの設定
	InitTransform(&_this->transform);

	AddTransformChild(parent, &_this->transform);
	if (g_shadowNum < SHADOW_MAX)
		g_shadowList[g_shadowNum] = _this;

	g_shadowNum++;

}

//=============================================================================
// 終了処理
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
// 更新処理
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
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 減算合成
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

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ライトなし
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxVuffShadow, 0, sizeof(Vertex3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShadow->pDXTex);

	for (int i = 0; i < g_shadowNum; i++)
	{
		// ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_shadowList[i]->transform.mtxWorld);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, blend_src);
	pDevice->SetRenderState(D3DRS_DESTBLEND, blend_dest);

	//pDevice->SetRenderState(D3DRS_SRCBLEND, blend_src);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, blend_dest);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ライトあり
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexShadow(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxVuffShadow,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		Vertex3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxVuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定(ローカル座標)
		pVtx[0].pos = Vector3(-SHADOW_SIZE_X, 0.0f, SHADOW_SIZE_Z);
		pVtx[1].pos = Vector3(SHADOW_SIZE_X, 0.0f, SHADOW_SIZE_Z);
		pVtx[2].pos = Vector3(-SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z);
		pVtx[3].pos = Vector3(SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z);

		// 法線ベクトルの設定
		pVtx[0].nor = Vector3(-1.0f, 1.0f, 0.0f);
		pVtx[1].nor = Vector3(1.0f, 1.0f, 0.0f);
		pVtx[2].nor = Vector3(-1.0f, 1.0f, 0.0f);
		pVtx[3].nor = Vector3(1.0f, 1.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = Color(255, 255, 255, 60);

		// テクスチャ座標の設定
		pVtx[0].uv = Vector2(0.0f, 0.0f);
		pVtx[1].uv = Vector2(1.0f, 0.0f);
		pVtx[2].uv = Vector2(0.0f, 1.0f);
		pVtx[3].uv = Vector2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxVuffShadow->Unlock();
	}

	return S_OK;
}
