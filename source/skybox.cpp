//=============================================================================
//
// 背景処理 [skybox.cpp]
// Author : GP11A243 楊 子毅
//
//=============================================================================
#include "skybox.h"
#include "Camera.h"
#include "Texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const char* SkyboxTextureName[6] =
{
	"sky_front",
	"sky_back",
	"sky_left",
	"sky_right",
	"sky_top",
	"sky_down",
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSkybox(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkybox;
Texture *g_pTexSkybox[6];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkybox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点、情報の作成
	MakeVertexSkybox();

	// テクスチャの読み込み
	for (int i = 0; i < 6; i++)
		g_pTexSkybox[i] = LoadTexture(SkyboxTextureName[i]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkybox(void)
{
	for (int i = 0; i < 6; i++)
	{
		ReleaseTexture(&g_pTexSkybox[i]);
		SafeRelease(g_pVtxBuffSkybox);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSkybox(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkybox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX  mtxTranslate;
	Camera *camera = GetCamera();

	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&mtxTranslate);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, camera->posP.x, camera->posP.y, camera->posP.z);
	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxTranslate);
	
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSkybox, 0, sizeof(Vertex3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < 6; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexSkybox[i]->pDXTex);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*RECT_NUM_VERTEX, RECT_NUM_POLYGON);

	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSkybox(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX * 6,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffSkybox,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	Vertex3D *pVtxBuff, *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffSkybox->Lock(0, 0, (void**)&pVtxBuff, 0);

	for (int i = 0; i < 6; i++)
	{

		pVtx = pVtxBuff + 4 * i;

		// 法線ベクトルの設定
		pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = Vector3(0.0f, 0.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = Color(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].uv = Vector2(0.0f, 0.0f);
		pVtx[1].uv = Vector2(1.0f, 0.0f);
		pVtx[2].uv = Vector2(0.0f, 1.0f);
		pVtx[3].uv = Vector2(1.0f, 1.0f);

	}

	// 前
	pVtx = pVtxBuff;
	pVtx[0].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);


	// 後
	pVtx = pVtxBuff + 4;
	pVtx[0].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);

	// 左
	pVtx = pVtxBuff + 4 * 2;
	pVtx[0].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);

	// 右
	pVtx = pVtxBuff + 4 * 3;
	pVtx[0].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);


	// 上
	pVtx = pVtxBuff + 4 * 4;
	pVtx[0].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[1].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[2].pos = Vector3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[3].pos = Vector3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);


	// 下
	pVtx = pVtxBuff + 4 * 5;
	pVtx[0].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[1].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[2].pos = Vector3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[3].pos = Vector3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);

	// 頂点データをアンロックする
	g_pVtxBuffSkybox->Unlock();

	return S_OK;
}



