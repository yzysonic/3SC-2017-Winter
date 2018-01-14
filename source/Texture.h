#pragma once
#include "Direct3D.h"
#include "Vector.h"

#define TextureDir "../Data/Texture/"

struct Texture
{
	const char* name;
	const char* file_name;
	LPDIRECT3DTEXTURE9 pDXTex;	// テクスチャポインタ
	Vector2 size;				// テクスチャサイズ
	int divideX;				// テクスチャ内X分割数
	int divideY;				// テクスチャ内Y分割数
};

Texture* GetTexture(const char* name, const char* file_name = "", int divX = 1, int divY = 1);
void ReleaseTexture(const char* file_name);
void ReleaseTextureAll(void);