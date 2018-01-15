//=============================================================================
//
// �w�i���� [skybox.h]
// Author : GP11A243 �k �q�B
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Core.h"

#define	SKYBOX_SIZE_X		(50)
#define	SKYBOX_SIZE_Y		(50)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSkybox(void);
void UninitSkybox(void);
void UpdateSkybox(void);
void DrawSkybox(void);
void LoadSkyboxTexture(void);

#endif