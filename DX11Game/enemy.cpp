//************************************************************************************
// 
// 敵[enemy.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include "Enemy.h"
#include "debugproc.h"
#include "shadow.h"
#include "explosion.h"
#include "DwarfEffect.h"
#include "Sound.h"
#include "bullet.h"
#include "collision.h"
#include "Astar.h"
#include "GameObjManager.h"

//-------------------- 定数定義 --------------------
#define MODEL_ENEMY			"data/model/kobitored.fbx"

#define	VALUE_MOVE_ENEMY	(0.50f)		// 移動速度
#define	RATE_MOVE_ENEMY		(0.20f)		// 移動慣性係数
#define	VALUE_ROTATE_ENEMY	(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY	(0.20f)		// 回転慣性係数
#define ENEMY_RADIUS		(50.0f)		// 境界球半径

#define BULLET_TIME			(120)		// 弾発射までの時間
#define ROOT_TIME			(300)		// 次にルート検索するまでの時間
#define MOVE_TIME			(30)		// 次にルート検索するまでの時間


std::unique_ptr<CAssimpModel> Enemy::pMyModel;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Enemy::Enemy(int mapIndex_x, int mapindex_y) {
	mapIndex.x = mapIndex_x;
	mapIndex.y = mapindex_y;
}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Enemy::~Enemy() {

}

//====================================================================================
//
//				初期化
//
//====================================================================================
void Enemy::Init() {
	//----- モデルデータの読み込み -----
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		}
	}

	//----- 位置・回転・スケール・サイズなどオブジェクトに必要な初期設定 -----
	pos = XMFLOAT3(mapIndex.x * 80.0f - 640.0f, 0.0f, -mapIndex.y * 80.0f + 480.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f,0.0f);
	rotModel = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rotModel;
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);
	myTag = ENEMY;								// オブジェクト識別のタグ
	collType = Collision::DYNAMIC;				// 動的オブジェクト

	//----- 敵の初期設定 -----
	shadowNum = CreateShadow(pos, 25.0f);	// 丸影の生成

	use = true;								// 使用フラグ
	
	isCollision = true;						// 当たり判定を行うか
	
	bulletTimer = 300;						// 弾発射の間隔

	mapIndex.x = (pos.x + 640.0f) / 80.0f;	// 座標をマップ番号に変換した値
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	rootTimer = 0;							// ルート検索の間隔
	rootIndex = search_Root(mapIndex);		// ルートを検索して入れる
	rootIndexNum = rootIndex.end()-1;		// ルートの検索結果が後ろから入るので後ろを初期値とする
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Enemy::Uninit() {
	ReleaseShadow(shadowNum);

	rootIndex.clear();

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
void Enemy::Update() {
	// 未使用ならスキップ
	if (!use) {
		return;
	}

	//----- 変数初期化 -----
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f, 0.0f, 0.0f);


	//----- 当たり判定 -----
	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (hitList[i].myTag == BULLET_PLAYER) {
				// 弾と当たった時
				Destroy();
				return;
			}
		}
	}

	//----- 移動 -----
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	// 座標から現在のマップ番号を取得
	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	// 向かうルートと現在の位置を比較して移動量を決める
	if (mapIndex.x < (*rootIndexNum).x) {
		moveVal.x = VALUE_MOVE_ENEMY;
		rotDest.y = rotCamera.y - 90.0f;
	} else if (mapIndex.x > (*rootIndexNum).x) {
		moveVal.x = -VALUE_MOVE_ENEMY;
		rotDest.y = rotCamera.y + 90.0f;
	} else if (mapIndex.x == (*rootIndexNum).x) {
		moveVal.x = 0.0f;
	}

	if (mapIndex.y < (*rootIndexNum).y) {
		moveVal.z = -VALUE_MOVE_ENEMY;
		rotDest.y = rotCamera.y;
	} else if (mapIndex.y > (*rootIndexNum).y) {
		moveVal.z = VALUE_MOVE_ENEMY;
		rotDest.y = rotCamera.y + 180.0f;
	} else if (mapIndex.y == (*rootIndexNum).y) {
		moveVal.z = 0.0f;
	}

	// 次のルートに更新
	if (mapIndex.x == (*rootIndexNum).x && mapIndex.y == (*rootIndexNum).y){
		if (rootIndex.size() > 1 && (rootIndexNum->x > -1 && rootIndexNum->y > -1)) {
			rootIndexNum--;
		}
	}

	// 目的の角度までの差分
	float fDiffRotY = rotDest.y - rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	rotModel.y += fDiffRotY * RATE_ROTATE_ENEMY;
	if (rotModel.y >= 180.0f) {
		rotModel.y -= 360.0f;
	}
	if (rotModel.y < -180.0f) {
		rotModel.y += 360.0f;
	}

	// 移動量の加算
	pos.x += moveVal.x;
	pos.z += moveVal.z;


	//----- 移動をワールド座標に変換する -----
	// ワールドマトリックスの初期化
	_mtxWorld = XMMatrixIdentity();

	/* ワールド座標の変換は必ず、拡大、回転、移動の順で行う */

	// スケールを反映
	_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
	_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);

	// 回転を反映
	_mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(rotModel.x),
		XMConvertToRadians(rotModel.y),
		XMConvertToRadians(rotModel.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// 移動を反映
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&mtxWorld, _mtxWorld);


	//----- 丸影の移動 -----
	if (moveVal.x > 0.0f) {
		ShadowMove.x += 25.0f;
	} else if (moveVal.x < 0.0f) {
		ShadowMove.x -= 25.0f;
	}

	if (moveVal.z > 0.0f) {
		ShadowMove.z += 25.0f;
	} else if (moveVal.z < 0.0f) {
		ShadowMove.z -= 25.0f;
	}

	MoveShadow(shadowNum, XMFLOAT3(pos.x + ShadowMove.x, pos.y, pos.z + ShadowMove.z));

	//----- 弾発射 -----
	int randomtime;
	randomtime = rand() % 3;
	bulletTimer -= randomtime;
	if (bulletTimer < 0) {
		Bullet::FireBullet(
			pos,
			XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLET_ENEMY,
			gameObjNum);

		bulletTimer = BULLET_TIME;
	}

	//----- ルート検索 -----
	rootTimer--;
	if (rootTimer < 0) {
	// マップの要素番号であったら現在の位置がどこになるのかを求める
		mapIndex.x = (pos.x + 640.0f) / 80.0f;
		mapIndex.y = abs(pos.z - 480.0) / 80.0f;
		rootIndex = search_Root(mapIndex);
		rootIndexNum = rootIndex.end()-1;
		rootTimer = ROOT_TIME;
	}
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Enemy::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	if (!use) {
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
}

//====================================================================================
//
//				消える
//
//====================================================================================
void Enemy::Destroy() {
	use = false;

	// 丸影解放
	ReleaseShadow(shadowNum);
	shadowNum = -1;
	// 爆発開始
	pos.x -= moveVal.x;
	pos.y -= moveVal.y;
	pos.z -= moveVal.z;

	int nExp = StartDwarfEffect(pos);

	isCollision = false;

	GameObjManager::DelList(gameObjNum, false);		// Uninitがモデルと丸影の開放のみなのでなし。ちょい上で丸影の開放は行った、
																// モデルの開放はシングルトンの為別の敵描画に影響が出るため行わない
}


//====================================================================================
//
//				次に移動するべき場所を格納したindexを保存する
//
//====================================================================================
void Enemy::SetRootIndex(XMINT2 index) {
	mapIndex = index;
}

//====================================================================================
//
//				A*の開始地点をいれる
//
//====================================================================================
XMINT2 Enemy::SetStartIndex() {
	return mapIndex;
}

//====================================================================================
//
//				使用フラグの取得
//
//====================================================================================
bool Enemy::GetUse() {
	return use;
}