//=============================================================================
//
// カメラ処理 [Camera.h]
// Author : GP11A243 楊 子毅
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"

#define	POS_X_CAM		(0.0f)						// カメラの初期位置(X座標)
#define	POS_Y_CAM		(15.0f)						// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-50.0f)					// カメラの初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))		// ビュー平面の視野角
#define	VIEW_NEAR_Z		(10.0f)						// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(5000.0f)					// ビュー平面のFarZ値

typedef struct _Camera
{
	Vector3 posP;				// カメラの視点
	Vector3 posR;				// カメラの注視点
	Vector3 vecU;				// カメラの上方向ベクトル
	Vector3 *target;			//ターゲット
	D3DXMATRIX	mtxView;			// ビューマトリックス
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス

	float phi;				// 注視点とのφ回転角（現在）
	float theta;			// 注視点とのθ回転角（現在）
	float move_phi;			// φ回転角の移動量
	float move_theta;		// θ回転角の移動量
	float target_dis;		// 目標距離
	float dis;				// 注視点との距離
	bool follow_mode;		// フォローモードフラグ
}Camera;

Camera* GetCamera(void);
void InitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);
D3DXMATRIX GetMtxView(void);

#endif