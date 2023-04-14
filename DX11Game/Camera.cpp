//************************************************************************************
// 
// カメラ[camera.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include "Camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//-------------------- グローバル変数定義 --------------------
namespace {
	const float CAM_POS_P_X = -40.0f;				// カメラの視点初期位置(X座標)
	const float CAM_POS_P_Y = 1000.0f;				// カメラの視点初期位置(Y座標)
	const float CAM_POS_P_Z = -350.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = -40.0f;				// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 50.0f;				// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)
	const float VIEW_ANGLE = 45.0f;					// ビュー平面の視野角
	const float VIEW_ASPECT = (float)SCREEN_WIDTH / SCREEN_HEIGHT;	// ビュー平面のアスペクト比
	const float VIEW_NEAR_Z = 10.0f;				// ビュー平面のNearZ値
	const float VIEW_FAR_Z = 5000.0f;				// ビュー平面のFarZ値
	const float VALUE_MOVE_CAMERA = 2.0f;			// カメラの移動量
	const float VALUE_ROTATE_CAMERA = 1.8f;			// カメラの回転量
	const float RATE_ROTATE_CAMERA = 0.20f;			// カメラの注視点への補正係数

	const float INTERVAL_CAMERA_R = 12.5f;			// モデルの視線の先までの距離
	const float RATE_CHASE_CAMERA_P = 0.35f;		// カメラの視点への補正係数
	const float RATE_CHASE_CAMERA_R = 0.20f;		// カメラの注視点への補正係数

	const float CHASE_HEIGHT_P = 100.0f;			// 追跡時の視点の高さ
	const float CHASE_HEIGHT_R = 10.0f;				// 追跡時の注視点の高さ

	CCamera g_camera;								// カメラ インスタンス
}

CCamera* CCamera::m_pCamera = &g_camera;			// 現在のカメラ

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
CCamera::CCamera()
{
	Init();
}

//====================================================================================
//
//				初期化
//
//====================================================================================
void CCamera::Init()
{
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);							// 上方ベクトル

	m_fAspectRatio = VIEW_ASPECT;		// 縦横比
	m_fFovY = VIEW_ANGLE;				// 視野角(単位:Degree)
	m_fNearZ = VIEW_NEAR_Z;				// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;				// 後方クリップ距離

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マトリックス計算
	CalcWorldMatrix();

	// マトリックス更新
	UpdateMatrix();
}

//====================================================================================
//
//				ビュープロジェクション、マトリックス更新
//
//====================================================================================
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

//====================================================================================
//
// 				ワールドマトリックス更新
//
//====================================================================================
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

//====================================================================================
//
//				視点/注視点/上方ベクトルからワールド マトリックス算出
//
//====================================================================================
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}
