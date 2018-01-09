#include "System.h"
#include "Direct3D.h"
#include "Window.h"
#include "Input.h"
#include "Sound.h"


//=============================================================================
// �V�X�e���̏���������
//=============================================================================
HRESULT InitSystem()
{
	HRESULT hr;

	// �E�B���h�E�̏�����
	hr = InitWindow();

	// �O���t�B�b�N�X�̏�����
	if (SUCCEEDED(hr))
		hr = InitD3D(GetHWnd(), GetWindowMode());

	// �C���v�b�g�̏�����
	if (SUCCEEDED(hr))
		hr = InitInput(GetHInstance(), GetHWnd());

	// �T�E���h�̏�����
	if (SUCCEEDED(hr))
		hr = InitSound(GetHWnd());

	return hr;
}

//=============================================================================
// �V�X�e���̏I������
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	UninitD3D();
	UninitWindow();
}