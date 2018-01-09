#include "Texture.h"
#include "Common.h"
#include "Window.h"

Texture* LoadTexture(const char* name, const char* file_name, int divX, int divY)
{
	char file_dir[256];
	char temp[256];

	if (file_name == "")
	{
		strcpy(temp, name);
		strcat(temp, ".png");
	}
	else
		strcpy(temp, file_name);

	strcat(strcpy(file_dir, TextureDir), temp);

	Texture *texture = (Texture*) malloc(sizeof(Texture));

	//テクスチャ詳細情報取得
	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(file_dir, &info) == D3D_OK)
	{
		//テクスチャ読込
		D3DXCreateTextureFromFileEx(
			GetDevice(),
			file_dir,
			info.Width, info.Height,
			1, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xFF000000,
			NULL, NULL,
			&texture->pDXTex);
	}
	else
	{
		TCHAR s[128];
		wsprintf(s, _T("テクスチャー「%s」の読込に失敗しました。"), file_name);
		MessageBox(GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);

		free(texture);

		return NULL;
	}

	texture->name = name;
	texture->file_name = file_name;
	texture->size.x = (float)info.Width / divX;
	texture->size.y = (float)info.Height / divY;
	texture->divideX = divX;
	texture->divideY = divY;

	return texture;
}

void ReleaseTexture(Texture **tex)
{
	if (*tex == NULL) return;
	SafeRelease((*tex)->pDXTex);
	SafeFree(*tex);
}
