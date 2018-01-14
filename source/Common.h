//=============================================================================
//
// ファイル名：common.h
// 作者：GP11A143 38 楊子毅
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

//システムのパラメータ
//数値の変更に反映するのはシステム初期化するときのみ。
struct SystemParameters
{
	static char* windowTitle;	// ウィンドウタイトル
	static int ResolutionX;		// 画面の解像度X
	static int ResolutionY;		// 画面の解像度Y
	static int FPS;				// 最大FPS
};

// メモリリークの自動検出
#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define _new(type) (type*)malloc(sizeof(type));
#define _newarray(type, size) (type*)malloc(sizeof(type)*size);
#define SafeRelease(ptr)	if(ptr != NULL) {ptr->Release(); ptr = NULL; }
#define SafeFree(ptr)	if(ptr != NULL) {free(ptr); ptr = NULL; }
#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)

