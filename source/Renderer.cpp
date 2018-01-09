#include "Renderer.h"
#include "SceneManager.h"

void InitRendererState(void);

LPD3DXFONT	g_pFont = NULL;

void InitRendererState(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// レンダリングステートパラメータの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// カリングを行わない
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

																		// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

																		// テクスチャステージ加算合成処理
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数

}


void InitRenderer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	InitRendererState();
}


void DrawFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		// メイン描画処理
		DrawScene();

		// デバッグ
		//DrawDebug();

		// Direct3Dによる描画の終了
		pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	HRESULT hr;
	hr = pDevice->Present(NULL, NULL, NULL, NULL);

	// デバイスロストの検知
	if (hr == D3DERR_DEVICELOST) {

		// 復帰可能の場合
		if (pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetRenderer();
		}
	}

}

void UninitRenderer(void)
{
}

bool ResetRenderer()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pFont->OnLostDevice();

	if(ResetDevice() == false) return false;

	g_pFont->OnResetDevice();

	InitRendererState();

	return true;
}

LPD3DXFONT GetFont()
{
	return g_pFont;
}
