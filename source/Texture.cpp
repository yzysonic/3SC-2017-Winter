#include "Texture.h"
#include "Common.h"
#include "Window.h"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, Texture*> g_textureMap;

Texture* GetTexture(const char* name, const char* file_name, int divX, int divY)
{
	char file_dir[256];
	char fName[256];

	if (file_name == "")
	{
		strcpy(fName, name);
		strcat(fName, ".png");
	}
	else
		strcpy(fName, file_name);

	try
	{
		return g_textureMap.at(std::string(fName));
	}
	catch (std::out_of_range){}

	strcat(strcpy(file_dir, TextureDir), fName);

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
		wsprintf(s, _T("テクスチャー「%s」の読込に失敗しました。"), fName);
		MessageBox(GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);

		free(texture);

		return NULL;
	}

	texture->name = name;
	texture->file_name = fName;
	texture->size.x = (float)info.Width / divX;
	texture->size.y = (float)info.Height / divY;
	texture->divideX = divX;
	texture->divideY = divY;

	g_textureMap[fName] = texture;

	return texture;
}

void ReleaseTexture(const char* file_name)
{
	if (file_name == NULL) return;

	Texture *tex;
	try
	{
		tex = g_textureMap.at(file_name);
	}
	catch (std::out_of_range) 
	{
		return;
	}
	
	SafeRelease(tex->pDXTex);
	SafeFree(tex);

	g_textureMap.erase(file_name);
}

void ReleaseTextureAll(void)
{
	if (g_textureMap.size() == 0)
		return;

	for (auto pair : g_textureMap)
	{
		Texture* tex = pair.second;
		SafeRelease(tex->pDXTex);
		SafeFree(tex);
	}

	g_textureMap.clear();
}
