//=============================================================================
//
// �t�@�C�����Fcommon.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <windows.h>
#include <d3dx9.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include "Math.h"
#include "Vector.h"

// ���������[�N�̎������o
#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//�V�X�e���̃p�����[�^
//���l�̕ύX�ɔ��f����̂̓V�X�e������������Ƃ��̂݁B
struct SystemParameters
{
	static char* windowTitle;	// �E�B���h�E�^�C�g��
	static int ResolutionX;		// ��ʂ̉𑜓xX
	static int ResolutionY;		// ��ʂ̉𑜓xY
	static int FPS;				// �ő�FPS
	static int SystemParameters::ObjectMax;		// �ő�I�u�W�F�N�g��
};

#define SafeRelease(ptr)	if(ptr != NULL) {ptr->Release(); ptr = NULL; }
#define SafeFree(ptr)	if(ptr != NULL) {free(ptr); ptr = NULL; }
#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)

