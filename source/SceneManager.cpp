#include "SceneManager.h"
#include "SceneGlobal.h"
#include "SceneGame.h"

void UninitCurrentScene(void);

static Scene g_current_scene = SceneNone;

void InitScene(void)
{
	// �O���[�o���V�[���̏�����
	InitSceneGlobal();
}

void UpdateScene(void)
{
	// �O���[�o���V�[���̍X�V
	UpdateSceneGlobal();

	// �J�����g�V�[���̍X�V
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		UpdateSceneGame();
		break;
	case SceneResult:

		break;
	}
}

void DrawScene(void)
{
	// �O���[�o���V�[���̕`��
	DrawSceneGlobal();

	// �J�����g�V�[���̕`��
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		DrawSceneGame();
		break;
	case SceneResult:

		break;
	}

}

void SetScene(Scene scene)
{
	// �J�����g�V�[���̏I������
	UninitCurrentScene();

	// �V�����V�[���̏�����
	switch (scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		InitSceneGame();
		break;
	case SceneResult:

		break;
	}

	g_current_scene = scene;
}

void UninitScene(void)
{
	UninitCurrentScene();	// �J�����g�V�[���̏I������
	UninitSceneGlobal();	// �O���[�o���V�[���̏I������
}

void UninitCurrentScene(void)
{
	switch (g_current_scene)
	{
	case SceneTitle:

		break;
	case SceneGame:
		UninitSceneGame();
		break;
	case SceneResult:

		break;
	}

	g_current_scene = SceneNone;
}
