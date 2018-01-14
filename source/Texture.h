#pragma once
#include "Direct3D.h"
#include "Vector.h"

#define TextureDir "../Data/Texture/"

struct Texture
{
	const char* name;
	const char* file_name;
	LPDIRECT3DTEXTURE9 pDXTex;	// �e�N�X�`���|�C���^
	Vector2 size;				// �e�N�X�`���T�C�Y
	int divideX;				// �e�N�X�`����X������
	int divideY;				// �e�N�X�`����Y������
};

Texture* GetTexture(const char* name, const char* file_name = "", int divX = 1, int divY = 1);
void ReleaseTexture(const char* file_name);
void ReleaseTextureAll(void);