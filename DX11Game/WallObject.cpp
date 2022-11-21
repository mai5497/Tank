//************************************************************************************
//
// 壁オブジェクト [WallObject.cpp]
// Author : 伊地田真衣
//
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "WallObject.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
//#include "explosion.h"
#include "player.h"
#include "DebugCollision.h"
#include "enemy.h"
#include "Astar.h"
#include "DwarfEffect.h"

//-------------------- マクロ定義 --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define	WALLOBJ_RADIUS		(55.0f)		// 境界球半径

//-------------------- グローバル変数定義 --------------------
static XMFLOAT3		g_rotModel;		// 現在の向き
static XMFLOAT3		g_rotDestModel;	// 目的の向き

GameObject *g_wallobj[MAPWIDTH * MAPHEIGHT];

int g_wallMap[MAPHEIGHT][MAPWIDTH] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
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
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			g_wallobj[j* MAPWIDTH +i] = new GameObject;
			g_wallobj[j* MAPWIDTH +i]->size = XMFLOAT3(25.0f, 25.0f, 25.0f);
			g_wallobj[j* MAPWIDTH +i]->moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_wallobj[j* MAPWIDTH +i]->collRadius = WALLOBJ_RADIUS;

			if (g_wallMap[j][i] == 0) {
				continue;
			}
			g_wallobj[j* MAPWIDTH +i]->pos = XMFLOAT3(i*80.0f - 640.0f, 0.0f, -j*80.0f + 480.0f);
			g_wallobj[j * MAPWIDTH + i]->mapIndex.x = i;
			g_wallobj[j * MAPWIDTH + i]->mapIndex.y = j;
		}
	}

	// 位置・回転・スケールの初期設定
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_size = XMFLOAT3(25.0f, 25.0f, 25.0f);

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScale = XMMatrixScaling(
				g_wallobj[j * MAPWIDTH + i]->size.x,
				g_wallobj[j * MAPWIDTH + i]->size.y,
				g_wallobj[j * MAPWIDTH + i]->size.z);

			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
				XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(
				g_wallobj[j * MAPWIDTH + i]->pos.x,
				g_wallobj[j * MAPWIDTH + i]->pos.y,
				g_wallobj[j * MAPWIDTH + i]->pos.z);

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_wallobj[j * MAPWIDTH + i]->mtxWorld, mtxWorld);
		}
	}


	// モデルデータの読み込み
	if (!model.Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
		MessageBoxA(GetMainWnd(), "壁モデルデータ読み込みエラー", "InitModel", MB_OK);
	}

	int* pMap = (int*)g_wallMap;
	SetMap(pMap);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void WallObj::Uninit() {
	// 壁オブジェクトの解放
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			delete g_wallobj[j * MAPWIDTH + i];
		}
	}

	// モデルの解放
	model.Release();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void WallObj::Update() {
	// 当たり判定
	//CollisionWallObj();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void WallObj::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}

			// 不透明部分を描画
			model.Draw(pDC, g_wallobj[j * MAPWIDTH + i]->mtxWorld, eOpacityOnly);

			// 半透明部分を描画
			SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
			SetZWrite(false);				// Zバッファ更新しない
			model.Draw(pDC, g_wallobj[j * MAPWIDTH + i]->mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Zバッファ更新する
			SetBlendState(BS_NONE);			// アルファブレンド無効

			// デバッグ表示
			DrawCollisionSphere(*g_wallobj[j * MAPWIDTH + i]);
		}
	}
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

