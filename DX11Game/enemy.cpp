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

#define	VALUE_MOVE_ENEMY	(0.40f)		// 移動速度
#define	RATE_MOVE_ENEMY		(0.20f)		// 移動慣性係数
#define	VALUE_ROTATE_ENEMY	(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY	(0.20f)		// 回転慣性係数
#define ENEMY_RADIUS		(50.0f)		// 境界球半径

#define BULLET_TIME			(300)		// 弾発射までの時間
#define ROOT_TIME			(300)		// 次にルート検索するまでの時間


std::unique_ptr<CAssimpModel> Enemy::pMyModel;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Enemy::Enemy() {
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
	// モデルデータの読み込み
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		}
	}

	// 位置・回転・スケール・サイズの初期設定
	pos = XMFLOAT3(rand() % 620 - 310.0f, 0.0f, rand() % 620 - 310.0f);
	moveVal = XMFLOAT3(
		-SinDeg(rotModel.y) * VALUE_MOVE_ENEMY,
		0.0f,
		-CosDeg(rotModel.y) * VALUE_MOVE_ENEMY);
	rotModel = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rotModel;
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);
	// 丸影の生成
	shadowNum = CreateShadow(pos, 25.0f);
	use = true;
	isCollision = true;
	bulletTimer = 300;

	mapIndex = XMINT2(0, 0);

	myTag = ENEMY;
	collType = Collision::DYNAMIC;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Enemy::Uninit() {
	ReleaseShadow(shadowNum);

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
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (!use) {
		return;
	}

	// 当たり判定
	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (hitList[i] == BULLET_PLAYER) {
				// 弾と当たった時
				Destroy();
				return;
			}
		}
	}

	moveVal = XMFLOAT3(
		-SinDeg(rotModel.y) * VALUE_MOVE_ENEMY,
		0.0f,
		-CosDeg(rotModel.y) * VALUE_MOVE_ENEMY);

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

	// 移動
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	// 壁にぶつかった
	bool lr = false, fb = false;
	if (pos.x < -310.0f) {
		pos.x = -310.0f;
		lr = true;
	}
	if (pos.x > 310.0f) {
		pos.x = 310.0f;
		lr = true;
	}
	if (pos.z < -310.0f) {
		pos.z = -310.0f;
		fb = true;
	}
	if (pos.z > 310.0f) {
		pos.z = 310.0f;
		fb = true;
	}
	if (pos.y < 0.0f) {
		pos.y = 0.0f;
	}
	if (pos.y > 80.0f) {
		pos.y = 80.0f;
	}
	if (fabsf(rotModel.y - rotDest.y) < 0.0001f) {
		if (lr) {
			moveVal.x *= -1.0f;
		}
		if (fb) {
			moveVal.z *= -1.0f;
		}
		if (lr || fb) {
			rotDest.y = XMConvertToDegrees(atan2f(-moveVal.x, -moveVal.z));
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


	// マップの要素番号であったら現在の位置がどこになるのかを求める
	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;


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

	// 丸影の移動
	MoveShadow(shadowNum, XMFLOAT3(pos.x + ShadowMove.x, pos.y, pos.z + ShadowMove.z));

	int randomtime;
	randomtime = rand() % 3;
	bulletTimer -= randomtime;
	if (bulletTimer < 0) {
		Bullet::FireBullet(
			pos,
			XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLET_ENEMY);

		bulletTimer = BULLET_TIME;
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

	GameObjManager::DelList(gameObjNum,false);		// Uninitがモデルと丸影の開放のみなのでなし。286で丸影の開放は行った、
																// モデルの開放はシングルトンの為別の敵描画に影響が出るため行わない
}

//====================================================================================
//
//				当たり判定
//
//====================================================================================
//int CollisionEnemy(XMFLOAT3 pos, float radius, float damage) {
//	int hitNum = 0;
//	for (int i = 0; i < MAX_ENEMY; i++) {
//		if (!g_enemy[i].m_use) {
//			continue;
//		}
//		bool hit = CollisionSphere(g_GameObj[i]->pos, ENEMY_RADIUS, pos, radius);
//		if (hit) {
//			hitNum++;
//			// 爆発開始
//			int nExp = -1;
//			if (damage > 0.0f) {
//				StartDwarfEffect(g_GameObj[i]->pos);
//				//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
//
//			} else {
//				nExp = StartExplosion(g_GameObj[i]->pos, XMFLOAT2(20.0f, 20.0f));
//			}
//			SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
//
//			g_enemyKillSum++;
//			g_enemy[i].m_use = false;
//			ReleaseShadow(g_enemy[i].m_nShadow);
//			CSound::Play(SE_KILL);
//		}
//	}
//	return hitNum;
//}
//
//void CollisionEnemy(GameObject collision) {
//	GameObject* _GameObject;
//
//	for (int i = 0; i < MAX_ENEMY; i++) {
//		bool isHit = CollisionSphere(*g_GameObj[i], collision);
//
//		if (isHit) {
//			_GameObject = Push(g_GameObj[i]->pos, XMFLOAT3(10.0f, 10.0f, 10.0f), g_GameObj[i]->moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));
//
//			if (_GameObject == nullptr) {
//				//　押し出しに失敗している
//				return;
//			}
//
//			g_GameObj[i]->pos = _GameObject->pos;
//			g_GameObj[i]->moveVal = _GameObject->moveVal;
//		}
//	}
//}



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