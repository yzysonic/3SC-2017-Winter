//=============================================================================
//
// �J�������� [Camera.cpp]
// Author : GP11A243 �k �q�B
//
//=============================================================================

#include "Camera.h"
#include "input.h"
#include "model.h"

Camera		g_camera;			// �J�����̍\���̕ϐ�
float		g_view_aspect;		// �r���[���ʂ̃A�X�y�N�g��

void MoveCamera(void);

Camera * GetCamera(void)
{
	return &g_camera;
}

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_camera.posP = Vector3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_camera.posR = Vector3(0.0f, POS_Y_CAM, 0.0f);
	g_camera.vecU = Vector3(0.0f, 1.0f, 0.0f);
	g_camera.theta = atan2f(sqrtf(g_camera.posP.x*g_camera.posP.x + g_camera.posP.z*g_camera.posP.z), g_camera.posP.y);
	g_camera.phi = atan2f(g_camera.posP.z, g_camera.posP.x);
	g_camera.move_theta = 0.0f;
	g_camera.move_phi = 0.0f;
	g_camera.dis = 500.0f;
	g_camera.target_dis = g_camera.posP.length();
	g_camera.follow_mode = true;
	g_view_aspect = (float)SystemParameters::ResolutionX / (float)SystemParameters::ResolutionY;

	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);
	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, VIEW_ANGLE, g_view_aspect, VIEW_NEAR_Z, VIEW_FAR_Z);

}

void UpdateCamera(void)
{

#ifdef _DEBUG
	MoveCamera();
#endif

	// �t�H���[���[�h�̐؂�ւ�
	if (GetKeyboardTrigger(DIK_F))
		g_camera.follow_mode = !g_camera.follow_mode;

	// �J�����̃��Z�b�g
	if (GetKeyboardTrigger(DIK_0))
		InitCamera();

	// �J�����̈ړ�����
	if (g_camera.target && g_camera.follow_mode == true)
		g_camera.posR = g_camera.posR + (*g_camera.target - g_camera.posR)*0.1f;

	// ���W�ϊ�
	g_camera.posP.y = g_camera.dis * cosf(g_camera.theta);
	g_camera.posP.x = g_camera.dis * sinf(g_camera.theta) * cosf(g_camera.phi);
	g_camera.posP.z = g_camera.dis * sinf(g_camera.theta) * sinf(g_camera.phi);
	g_camera.posP += g_camera.posR;

}

void DrawCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);
	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, (D3DXVECTOR3*)&g_camera.posP, (D3DXVECTOR3*)&g_camera.posR, (D3DXVECTOR3*)&g_camera.vecU);
	// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

D3DXMATRIX GetMtxView(void)
{
	return g_camera.mtxView;
}

void MoveCamera(void)
{
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);

	// �}�E�X����
	if (IsMouseCenterPressed())
	{
		// �ړ�
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			move.x = -GetMouseMoveX() / 5.0f;
			move.y = GetMouseMoveY() / 5.0f;
		}
		// ��]
		else
		{
			g_camera.move_phi = -GetMouseMoveX() / 500.0f;
			g_camera.move_theta = -GetMouseMoveY() / 500.0f;
		}
	}

	// �Y�[��
	g_camera.target_dis += -GetMouseMoveZ() / 3.0f;
	g_camera.dis = g_camera.dis + (g_camera.target_dis - g_camera.dis)*0.15f;


	// �L�[�{�[�h����
	// �ړ�
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		if (GetKeyboardPress(DIK_LEFT))
			move.x = -2.0f;
		if (GetKeyboardPress(DIK_RIGHT))
			move.x = 2.0f;
		if (GetKeyboardPress(DIK_UP))
			move.y = 2.0f;
		if (GetKeyboardPress(DIK_DOWN))
			move.y = -2.0f;
	}
	// ��]
	else
	{
		if (GetKeyboardPress(DIK_LEFT))
			g_camera.move_phi = 0.02f;
		if (GetKeyboardPress(DIK_RIGHT))
			g_camera.move_phi = -0.02f;
		if (GetKeyboardPress(DIK_UP))
			g_camera.move_theta = 0.005f;
		if (GetKeyboardPress(DIK_DOWN))
			g_camera.move_theta = -0.005f;
	}


	if (g_camera.move_phi != 0.0f)
	{
		g_camera.phi += g_camera.move_phi;
		if (fabsf(g_camera.move_phi) >= 0.001f)
			g_camera.move_phi *= 0.92f;
		else
			g_camera.move_phi = 0.0f;
	}
	if (g_camera.move_theta != 0.0f)
	{
		g_camera.theta += g_camera.move_theta;
		if (fabsf(g_camera.move_theta) >= 0.001f)
			g_camera.move_theta *= 0.92f;
		else
			g_camera.move_theta = 0.0f;
	}


	if (move.x != 0.0f || move.y != 0.0f)
	{
		g_camera.follow_mode = false;

		float x = -move.x*cosf(-g_camera.phi + 0.5f*PI) - move.z*sinf(-g_camera.phi + 0.5f*PI);
		move.z = move.x*sinf(-g_camera.phi + 0.5f*PI) - move.z*cosf(-g_camera.phi + 0.5f*PI);
		move.x = x;

		g_camera.posR += move;
		g_camera.follow_mode = false;
	}


}
