#include "Renderer.h"
#include "SceneManager.h"

void InitRendererState(void);

LPD3DXFONT	g_pFont = NULL;

void InitRendererState(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// �J�����O���s��Ȃ�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

																		// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

																		// �e�N�X�`���X�e�[�W���Z��������
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����

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

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		// ���C���`�揈��
		DrawScene();

		// �f�o�b�O
		//DrawDebug();

		// Direct3D�ɂ��`��̏I��
		pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	HRESULT hr;
	hr = pDevice->Present(NULL, NULL, NULL, NULL);

	// �f�o�C�X���X�g�̌��m
	if (hr == D3DERR_DEVICELOST) {

		// ���A�\�̏ꍇ
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
