//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "System.h"
#include "Game.h"
#include "SceneManager.h"


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // ���������[�N�������o
#endif

	SystemParameters::windowTitle = "��������";

	// ������
	if (FAILED(InitSystem()))
		return 1;

	// �}�E�X���B��
	//ShowCursor(false);

	// �Q�[��������
	InitGame();
#ifndef _DEBUG
	SetScene(SceneTitle);
#else
	SetScene(SceneTitle);
#endif

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}
