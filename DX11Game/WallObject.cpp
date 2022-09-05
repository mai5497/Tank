//************************************************************************************
//
// 壁オブジェクト [WallObject.cpp]
// Author : 伊地田真衣
//
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "WallObject.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"

//-------------------- マクロ定義 --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define	WALLOBJ_RADIUS		(10.0f)		// 境界球半径

//-------------------- グローバル変数定義 --------------------
static CAssimpModel	g_model;		// モデル

static XMFLOAT3		g_posModel;		// 現在の位置
static XMFLOAT3		g_rotModel;		// 現在の向き
static XMFLOAT3		g_rotDestModel;	// 目的の向き
static XMFLOAT3		g_moveModel;	// 移動量
static XMFLOAT3		g_size;			// モデルの描画サイズ

static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス

static int			g_nShadow;		// 丸影番号

//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT InitWallObj(void) {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_size = XMFLOAT3(25.0f, 25.0f, 25.0f);

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
		MessageBoxA(GetMainWnd(), "自機モデルデータ読み込みエラー", "InitModel", MB_OK);
		return E_FAIL;
	}
	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void UninitWallObj(void) {
	// モデルの解放
	g_model.Release();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void UpdateWallObj(void) {
	//if (GetKeyPress(VK_RETURN)) {
	//	// リセット
	//	g_posModel = XMFLOAT3(0.0f, 40.0f, 0.0f);
	//	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScale = XMMatrixScaling(g_size.x, g_size.y, g_size.z);
	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);
}

//====================================================================================
//
//				描画
//
//====================================================================================
void DrawWallObj(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
}

//====================================================================================
//
//				位置取得
//
//====================================================================================
XMFLOAT3& GetWallObjPos() {
	return g_posModel;
}

//====================================================================================
//
//				衝突判定
//
//====================================================================================
void CollisionWallObj() {

}
