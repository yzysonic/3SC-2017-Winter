#include "Collider.h"
#include "Vector.h"

void CalulatePolyColliderNormal(PolyCollider * _this)
{
	_this->normal = Vector3::Cross(_this->vtx[1] - _this->vtx[0], _this->vtx[2] - _this->vtx[0]).normalized();
}

void InitMeshColliderByModel(MeshCollider *_this, const Model * model)
{
	_this->polyNum = model->pMesh->GetNumFaces();

	if (_this->polyNum < 1)
	{
		_this->polyList = NULL;
		return;
	}

	_this->polyList = (PolyCollider*)malloc(sizeof(PolyCollider)*_this->polyNum);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;
	Vector3 *pVtx;
	WORD *pIdx;

	model->pMesh->GetVertexBuffer(&pVtxBuff);
	model->pMesh->GetIndexBuffer(&pIdxBuff);

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int n = 0; n < _this->polyNum; n++)
	{
		_this->polyList[n].vtx[0] = pVtx[pIdx[n * 3 + 0]];
		_this->polyList[n].vtx[1] = pVtx[pIdx[n * 3 + 1]];
		_this->polyList[n].vtx[2] = pVtx[pIdx[n * 3 + 2]];
		_this->polyList[n].isTrigger = false;
		_this->polyList[n].isCollidinig = false;

		CalulatePolyColliderNormal(_this->polyList + n);
	}

	pVtxBuff->Unlock();
	pIdxBuff->Unlock();

}

void InitMeshColliderByPoly(MeshCollider * _this, const RectPolygon * poly)
{
	_this->polyNum = RECT_NUM_POLYGON;
	_this->polyList = (PolyCollider*)malloc(sizeof(PolyCollider)*RECT_NUM_POLYGON);

	Vertex3D *pVtx = LockRectPolygonVtx((RectPolygon*)poly);

	for (int n = 0; n < RECT_NUM_POLYGON; n++)
	{
		_this->polyList[n].vtx[0] = pVtx[n+0].pos;
		_this->polyList[n].vtx[1] = pVtx[n+1].pos;
		_this->polyList[n].vtx[2] = pVtx[n+2].pos;
		_this->polyList[n].isTrigger = false;
		_this->polyList[n].isCollidinig = false;

		CalulatePolyColliderNormal(_this->polyList + n);
	}

	UnlockRectPolygonVtx(poly);
}

void InitSphereCollider(SphereCollider * _this, Vector3 * pos)
{
	_this->radius = 1.0f;
	Model sphere;
	InitModel(&sphere, "sphere.x");
	InitMeshColliderByModel(&_this->sphere, &sphere);
	UninitModel(&sphere);
}

void UninitMeshCollider(MeshCollider * _this)
{
	SafeFree(_this->polyList);
}

void DrawPolyCollider(PolyCollider * _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD lighting_state;
	D3DXMATRIX mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(64, 219, 111, 200));
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	pDevice->GetRenderState(D3DRS_LIGHTING, &lighting_state);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pDevice->SetFVF(D3DFVF_XYZ);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, _this->vtx, sizeof(Vector3));

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pDevice->SetRenderState(D3DRS_LIGHTING, lighting_state);

}

void DrawMeshCollider(MeshCollider * _this)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD lighting_state;
	D3DXMATRIX mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(64, 219, 111, 200));
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	pDevice->GetRenderState(D3DRS_LIGHTING, &lighting_state);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pDevice->SetFVF(D3DFVF_XYZ);
	for (int i = 0; i < _this->polyNum; i++)
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, _this->polyList[i].vtx, sizeof(Vector3));

	
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	for (int i = 0; i < _this->polyNum; i++)
	{
		if (_this->polyList[i].isCollidinig)
		{
			pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 57, 63, 100));
			_this->polyList[i].isCollidinig = false;
		}
		else
			pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(181, 230, 29, 100));

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, _this->polyList[i].vtx, sizeof(Vector3));
	}

	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pDevice->SetRenderState(D3DRS_LIGHTING, lighting_state);


}

void DrawSphereCollider(SphereCollider * _this)
{
	DrawMeshCollider(&_this->sphere);
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//DWORD lighting_state;
	//D3DXMATRIX mtxWorld, temp;

	//D3DXMatrixIdentity(&mtxWorld);

	//D3DXMatrixScaling(&temp, _this->radius, _this->radius, _this->radius);
	//mtxWorld *= temp;

	//D3DXMatrixTranslation(&temp, _this->pos->x, _this->pos->y, _this->pos->z);
	//mtxWorld *= temp;

	//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(64, 219, 111, 200));
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	//pDevice->GetRenderState(D3DRS_LIGHTING, &lighting_state);
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//pDevice->SetFVF(D3DFVF_XYZ);
	//for (int i = 0; i < _this->sphere.polyNum; i++)
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, _this->sphere.polyList[i].vtx, sizeof(Vector3));


	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//for (int i = 0; i < _this->sphere.polyNum; i++)
	//{
	//	if (_this->sphere.polyList[i].isCollidinig)
	//	{
	//		pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 57, 63, 100));
	//		_this->sphere.polyList[i].isCollidinig = false;
	//	}
	//	else
	//		pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(181, 230, 29, 100));

	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, _this->sphere.polyList[i].vtx, sizeof(Vector3));
	//}

	//pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, lighting_state);

}

void UnintSphereCollider(SphereCollider * _this)
{
	UninitMeshCollider(&_this->sphere);
}
