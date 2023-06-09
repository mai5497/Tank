//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __CAMERA_H__
#define __CAMERA_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "SceneManager.h"

//-------------------- クラス定義 --------------------
class CCamera {
private:
	//---変数
	XMFLOAT3 m_vPos;				// 視点
	XMFLOAT3 m_vTarget;				// 注視点
	XMFLOAT3 m_vUp;					// 上方ベクトル

	XMFLOAT3 m_vAngle;				// カメラの角度
	
	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	XMFLOAT4X4 m_mtxWorld;			// ワールド マトリックス
	XMFLOAT4X4 m_mtxView;			// ビュー マトリックス
	XMFLOAT4X4 m_mtxProj;			// プロジェクション マトリックス

	static CCamera* m_pCamera;		// 現在のカメラ

public:
	//---関数
	CCamera();

	void Init();

	void SetPos(XMFLOAT3& vPos) { m_vPos = vPos; }
	XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	XMFLOAT3& GetUpVector() { return m_vUp; }
	void SetWorldMatrix(XMFLOAT4X4& mtxWorld);
	XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	XMFLOAT3& GetAngle() { return m_vAngle; }

	static CCamera* Get() { return m_pCamera; }
	static void Set(CCamera* pCamera) { m_pCamera = pCamera; }
};

#endif // !__CAMERA_H__