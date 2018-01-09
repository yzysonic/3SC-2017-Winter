#pragma once

#include <d3dx9.h>

struct Bone : public D3DXFRAME
{
	D3DXMATRIX	mtxOffset;
	D3DXMATRIX	mtxCombined;
};

void UpdateBoneMatrix(D3DXMATRIX parentMtx, Bone *bone);