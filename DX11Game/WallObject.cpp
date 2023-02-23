//************************************************************************************
//
// 壁オブジェクト [WallObject.cpp]
// Author : 伊地田真衣
//
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "WallObject.h"
#include "input.h"
#include "debugproc.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "player.h"
#include "DebugCollision.h"
#include "enemy.h"
#include "Astar.h"
#include "DwarfEffect.h"
#include "Texture.h"

//-------------------- マクロ定義 --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define BUMP_WALLOBJ		"data/model/rubikcube_bump.png"
#define	WALLOBJ_RADIUS		(55.0f)		// 境界球半径

std::unique_ptr<CAssimpModel> WallObj::pMyModel;
//int WallObj::wallMap[MAPHEIGHT][MAPWIDTH] = {
//	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//};

int WallObj::wallMap[MAPHEIGHT][MAPWIDTH] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};


//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
WallObj::WallObj() {

}

WallObj::WallObj(int x, int y) {
	mapIndex.x = x;
	mapIndex.y = y;
}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
WallObj::~WallObj() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void WallObj::Init() {
	// モデルデータの読み込み
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

		// バンプマップ読込
		//std::unique_ptr<Texture> _pTexture = std::make_unique<Texture>();		// テクスチャ
		//_pTexture->SetTexture(pDevice, BUMP_WALLOBJ);
		//pMyModel->SetShaderMode(CAssimpModel::SM_BUMP);	// バンプマップする		
		//pMyModel->SetShaderMat(_pTexture->GetTexture());
		////_pTexture->ReleaseTexture();
		//_pTexture.reset();

		//if (_pTexture) {
		//	pDeviceContext->PSSetShaderResources(4, 1, &_pTexture);
		//} else {
		//	MessageBoxA(GetMainWnd(), "バンプマップ画像読み込みエラー", "InitWallObj", MB_OK);
		//}

		// モデルデータ読込
		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
			MessageBoxA(GetMainWnd(), "壁モデルデータ読み込みエラー", "InitWallObj", MB_OK);
		}

		// マップの設定も一度行えばよいためモデル読込と同居させとく
		int* pMap = (int*)wallMap;
		SetMap(pMap);
	}

	if (wallMap[mapIndex.y][mapIndex.x] != 0) {
		// 位置・回転・スケールの初期設定
		size = XMFLOAT3(25.0f, 25.0f, 25.0f);
		moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		collRadius = WALLOBJ_RADIUS;
		collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);

		rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		isCollision = true;

		XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;

		pos = XMFLOAT3(mapIndex.x * 80.0f - 640.0f, 0.0f, -mapIndex.y * 80.0f + 480.0f);
		// ワールドマトリックスの初期化
		_mtxWorld = XMMatrixIdentity();

		// スケールを反映
		_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
		_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);

		// 回転を反映
		_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotModel.x), XMConvertToRadians(rotModel.y), XMConvertToRadians(rotModel.z));
		_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

		// 移動を反映
		_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
		_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&mtxWorld, _mtxWorld);
	}
	collType = Collision::STATIC;
	myTag = WALL;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void WallObj::Uninit() {
	// モデルの解放
	if (pMyModel) {
		pMyModel->Release();
		pMyModel.reset();
	}
}

//====================================================================================
//
//				更新
//
//====================================================================================
void WallObj::Update() {
}

//====================================================================================
//
//				描画
//
//====================================================================================
void WallObj::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	if (wallMap[mapIndex.y][mapIndex.x] == 0) {
		return;
	}

	// 不透明部分を描画
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効

	// デバッグ表示
	//DrawCollisionSphere(this);
}

//====================================================================================
//
//				位置取得
//
//====================================================================================
//XMFLOAT3& GetWallObjPos() {
//	return g_posModel;
//}

//====================================================================================
//
//				衝突判定
//
//====================================================================================
//void CollisionWallObj() {
//	//bool isHit;
//	for (int j = 0; j < MAPHEIGHT; j++) {
//		for (int i = 0; i < MAPWIDTH; i++) {
//			if (g_wallMap[j][i] == 0) {
//				continue;
//			}
//
//			//isHit = CollisionPlayer(g_wallobj[j * MAPNUM + i]->m_pos, g_wallobj[j * MAPNUM + i]->m_radius, g_wallobj[j * MAPNUM + i]->m_size);
//			
//			/*
//			* あとでオブジェクト同士の受け渡し書く！！！！！！
//			*/
//			
//			
//			//CollisionPlayer(*g_wallobj[j * MAPWIDTH + i]);
//			//CollisionEnemy(*g_wallobj[j * MAPWIDTH + i]);
//			
//			
//			
//			
//			
//			//if (isHit) {
//			//	return;
//			//}
//		}
//	}
//}

/// <summary>
/// 弾と壁の当たり判定
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
/// <param name="damage"></param>
/// <returns></returns>
//bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage) {
//	bool hit;
//	Collision _collision;
//	for (int j = 0; j < MAPHEIGHT; j++) {
//		for (int i = 0; i < MAPWIDTH; i++) {
//			if (g_wallMap[j][i] == 0) {
//				continue;
//			}
//			hit = _collision.CollisionSphere(g_wallobj[j][i].pos, 45, pos, radius);
//			if (hit) {
//				// 爆発開始
//				int nExp = -1;
//				if (damage > 0.0f) {
//					StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(40.0f, 40.0f));
//					//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
//
//				} else {
//					nExp = StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(20.0f, 20.0f));
//				}
//				SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
//			}
//		}
//	}
//	return hit;
//}

