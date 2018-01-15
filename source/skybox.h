//=============================================================================
//
// 背景処理 [skybox.h]
// Author : GP11A243 楊 子毅
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Core.h"

#define	SKYBOX_SIZE_X		(50)
#define	SKYBOX_SIZE_Y		(50)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSkybox(void);
void UninitSkybox(void);
void UpdateSkybox(void);
void DrawSkybox(void);
void LoadSkyboxTexture(void);

#endif