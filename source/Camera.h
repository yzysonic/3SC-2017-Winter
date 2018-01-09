//=============================================================================
//
// �J�������� [Camera.h]
// Author : GP11A243 �k �q�B
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"

#define	POS_X_CAM		(0.0f)						// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM		(15.0f)						// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM		(-50.0f)					// �J�����̏����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))		// �r���[���ʂ̎���p
#define	VIEW_NEAR_Z		(10.0f)						// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(5000.0f)					// �r���[���ʂ�FarZ�l

typedef struct _Camera
{
	Vector3 posP;				// �J�����̎��_
	Vector3 posR;				// �J�����̒����_
	Vector3 vecU;				// �J�����̏�����x�N�g��
	Vector3 *target;			//�^�[�Q�b�g
	D3DXMATRIX	mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X

	float phi;				// �����_�Ƃ̃Ӊ�]�p�i���݁j
	float theta;			// �����_�Ƃ̃Ɖ�]�p�i���݁j
	float move_phi;			// �Ӊ�]�p�̈ړ���
	float move_theta;		// �Ɖ�]�p�̈ړ���
	float target_dis;		// �ڕW����
	float dis;				// �����_�Ƃ̋���
	bool follow_mode;		// �t�H���[���[�h�t���O
}Camera;

Camera* GetCamera(void);
void InitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);
D3DXMATRIX GetMtxView(void);

#endif