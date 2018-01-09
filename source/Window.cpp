#include "Window.h"
#include "Common.h"
#include "Direct3D.h"
#include "Game.h"

static HINSTANCE g_hInstance = NULL;
static HWND g_hWnd = NULL;
static MSG  g_Msg;
static bool g_bWindowMode = true;
static char* g_ClassName = "MainWindow";

//=============================================================================
// Windows�̏���������
//=============================================================================
HRESULT InitWindow()
{
	g_hInstance = GetModuleHandle(NULL);
	int width = SystemParameters::ResolutionX;
	int height = SystemParameters::ResolutionY;

	// �E�B���h�E�N���X��o�^����
	WNDCLASSEX	wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = g_hInstance;
	wcex.lpszClassName = g_ClassName;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClassEx(&wcex);


	// �E�B���h�E�I�u�W�F�N�g���쐬����.
	g_hWnd = CreateWindowEx(
		0,							//�g���X�^�C���i�C�Ӂj
		g_ClassName,			//�N���X��
		SystemParameters::windowTitle,				//�E�B���h�E���i�^�C�g���j
		(WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU),		//�X�^�C��
		((GetSystemMetrics(SM_CXSCREEN) - width) / 2),		//�������̈ʒu
		((GetSystemMetrics(SM_CYSCREEN) - height) / 2),	//�c�����̈ʒu
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//��
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//����
		NULL,						//�e�E�B���h�E
		NULL,						//���j���[
		g_hInstance,				//�A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL						//�E�B���h�E�쐬�f�[�^
	);

	if (g_hWnd == NULL)
		return E_FAIL;

	//�E�B���h�E���X�N���[���ɕ\������
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	return S_OK;
}


//=============================================================================
// Windows�̏I������
//=============================================================================
void UninitWindow()
{
	// �E�B���h�E�̃f�X�g���C�h
	g_hWnd = NULL;

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(g_ClassName, g_hInstance);
	g_hInstance = NULL;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		if (EndGame())
		{
			DestroyWindow(GetHWnd());	// �E�B���h�E��j������悤�w������
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]�L�[�������ꂽ
			StopGame();
			break;
		case VK_F9:			// [F9]�L�[�������ꂽ
			SetWindowMode(!g_bWindowMode);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CheckMesg(void)
{
	if (PeekMessage(&g_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()���Ă΂ꂽ��Q�[���I��
		if (g_Msg.message == WM_QUIT)
		{
			StopGame();
			return;
		}
		// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
		else
		{
			TranslateMessage(&g_Msg);
			DispatchMessage(&g_Msg);
		}
	}

}


HWND GetHWnd()
{
	return g_hWnd;
}

HINSTANCE GetHInstance(void)
{
	return g_hInstance;
}

bool GetWindowMode(void)
{
	return g_bWindowMode;
}

void SetWindowMode(bool windowMode)
{
	if (!SetWindowModeD3D(windowMode))
		return;

	if (windowMode)
	{
		SetWindowPos(g_hWnd, NULL,
			(GetSystemMetrics(SM_CXSCREEN) - SystemParameters::ResolutionX) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - SystemParameters::ResolutionY) / 2,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	g_bWindowMode = windowMode;
}