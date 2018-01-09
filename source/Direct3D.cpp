//=============================================================================
//
// �t�@�C�����Fgraphics.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "Direct3D.h"
#include "Common.h"
#include <DxErr.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")


LPDIRECT3D9				g_pD3D = NULL;
LPDIRECT3DDEVICE9		g_pDevice = NULL;
D3DPRESENT_PARAMETERS	g_d3dpp = {};
HWND					g_hWnd = NULL;


//=============================================================================
// �O���t�B�b�N�̏���������
//=============================================================================
HRESULT InitD3D(HWND hWnd, bool bWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SystemParameters::ResolutionX;	// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SystemParameters::ResolutionY;	// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.hDeviceWindow = hWnd;							// �g�p����E�B���h
	d3dpp.Windowed = bWindowMode;						// �E�B���h�E���[�h�̐ݒ�
	d3dpp.EnableAutoDepthStencil = TRUE;				// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,								// �f�B�X�v���C�^�C�v
		hWnd,										// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,										// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	g_d3dpp = d3dpp;
	g_hWnd = hWnd;

	return S_OK;
}

//=============================================================================
// �O���t�B�b�N�̏I������
//=============================================================================
void UninitD3D()
{
	SafeRelease(g_pDevice);
	SafeRelease(g_pD3D);
}


LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pDevice;
}

bool ResetDevice(void)
{
	HRESULT hr = g_pDevice->Reset(&g_d3dpp);

	if (FAILED(hr))
	{
		MessageBox(g_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		hr = g_pDevice->TestCooperativeLevel();
		MessageBox(g_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

bool SetWindowModeD3D(bool windowMode)
{
	g_d3dpp.Windowed = windowMode;

	// �E�B���h�E���[�h
	if (windowMode)
	{
		g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// �o�b�N�o�b�t�@
		g_d3dpp.FullScreen_RefreshRateInHz = 0;							// ���t���b�V�����[�g
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	// �t���X�N���[�����[�h
	else
	{
		g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;						// �o�b�N�o�b�t�@
		g_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	return ResetDevice();
}
