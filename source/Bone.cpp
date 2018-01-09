#include "Bone.h"

void UpdateBoneMatrix(D3DXMATRIX parentMtx, Bone* bone)
{
	D3DXMATRIX mtxBone = bone->TransformationMatrix * parentMtx;
	bone->mtxCombined = bone->mtxOffset * mtxBone;
	if (bone->pFrameFirstChild)
		UpdateBoneMatrix(mtxBone, (Bone*)bone->pFrameFirstChild);
	if (bone->pFrameSibling)
		UpdateBoneMatrix(parentMtx, (Bone*)bone->pFrameSibling);
}