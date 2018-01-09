#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Random.h"
#include "Texture.h"

// �O���[�o���ϐ�
bool g_bRunGame = true;

// �Q�[��������
void InitGame()
{
	InitRenderer();
	InitTime();
	InitRandom();
	InitScene();
}

// �Q�[�����[�v
void RunGame(void)
{
	while (g_bRunGame)
	{
		CheckMesg();
		UpdateInput();
		UpdateScene();
		DrawFrame();
		FramerateControl();
	}
}

// �Q�[���I������
void UninitGame(void)
{
	UninitScene();
	UninitRenderer();
}

// �Q�[���I���m�F
bool EndGame(void)
{
	return true;
}

// �Q�[���I��
void StopGame(void)
{
	g_bRunGame = false;
}
