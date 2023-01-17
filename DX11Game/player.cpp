//************************************************************************************
//
// プレイヤー [player.cpp]
// Author : 伊地田真衣
//
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Player.h"
#include "input.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "DebugCollision.h"
#include "Texture.h"

//-------------------- マクロ定義 --------------------
#define MODEL_PLAYER	"data/model/kobitoblue.fbx"
#define TOON_TEXTURE	"data/model/ramp.png"

#define	VALUE_MOVE_PLAYER	(0.5f)	// 移動速度
#define	RATE_MOVE_PLAYER	(0.25f)	// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(4.5f)		// 回転速度
#define	RATE_ROTATE_PLAYER	(0.1f)		// 回転慣性係数

#define	PLAYER_RADIUS		(10.0f)		// 境界球半径

std::unique_ptr<CAssimpModel> Player::pMyModel;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Player::Player() {
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Player::~Player() {
}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Player::Init() {
	// 位置・回転・スケールの初期設定
	pos = XMFLOAT3(100.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);

	isCollision = true;

	// モデルデータの読み込み
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		// トゥーンシェーダー
		ID3D11ShaderResourceView* _pTexture = nullptr;		// テクスチャ
		CreateTextureFromFile(pDevice, TOON_TEXTURE, &_pTexture);
		pMyModel->SetShaderMode(CAssimpModel::SM_TOON);	// トゥーンシェーダーにする
		pMyModel->SetShaderMat(_pTexture);

		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
			MessageBoxA(GetMainWnd(), "プレイヤーモデルデータ読み込みエラー", "InitModel", MB_OK);
		}
	}
	// 丸影の生成
	shadowNum = CreateShadow(pos, 12.0f);

	myTag = PLAYER;
	collType = Collision::DYNAMIC;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Player::Uninit() {
	// 丸影の解放
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
void Player::Update() {
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (GetKeyPress(VK_A)) {
		if (GetKeyPress(VK_W)) {
			// 左前移動
			moveVal.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// 左後移動
			moveVal.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// 左移動
			moveVal.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_D)) {
		if (GetKeyPress(VK_W)) {
			// 右前移動
			moveVal.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// 右後移動
			moveVal.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// 右移動
			moveVal.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 90.0f;
		}
	} else if (GetKeyPress(VK_W)) {
		// 前移動
		moveVal.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
		moveVal.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;

		rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_S)) {
		// 後移動
		moveVal.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		moveVal.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;

		rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_J)) {
		// 左回転
		rotDestModel.y -= VALUE_ROTATE_PLAYER;
		if (rotDestModel.y < -180.0f) {
			rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_L)) {
		// 右回転
		rotDestModel.y += VALUE_ROTATE_PLAYER;
		if (rotDestModel.y >= 180.0f) {
			rotDestModel.y -= 360.0f;
		}
	}

	// 目的の角度までの差分
	float fDiffRotY = rotDestModel.y - rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (rotModel.y >= 180.0f) {
		rotModel.y -= 360.0f;
	}
	if (rotModel.y < -180.0f) {
		rotModel.y += 360.0f;
	}

	// 位置移動
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	// 移動量に慣性をかける
	moveVal.x += (0.0f - moveVal.x) * RATE_MOVE_PLAYER;
	moveVal.y += (0.0f - moveVal.y) * RATE_MOVE_PLAYER;
	moveVal.z += (0.0f - moveVal.z) * RATE_MOVE_PLAYER;


	if (pos.x < -630.0f) {
		pos.x = -630.0f;
	}
	if (pos.x > 630.0f) {
		pos.x = 630.0f;
	}
	if (pos.z < -630.0f) {
		pos.z = -630.0f;
	}
	if (pos.z > 630.0f) {
		pos.z = 630.0f;
	}
	if (pos.y < 10.0f) {
		pos.y = 10.0f;
	}
	if (pos.y > 150.0f) {
		pos.y = 150.0f;
	}

	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;

	// ワールドマトリックスの初期化
	_mtxWorld = XMMatrixIdentity();

	// スケールを反映
	_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
	_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);

	// 回転を反映
	_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotModel.x),
		XMConvertToRadians(rotModel.y), XMConvertToRadians(rotModel.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// 移動を反映
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&mtxWorld, _mtxWorld);

	// 丸影の移動
	MoveShadow(shadowNum, pos);

	if ((moveVal.x * moveVal.x
		+ moveVal.y * moveVal.y
		+ moveVal.z * moveVal.z) > 1.0f) {
		XMFLOAT3 effPos;

		effPos.x = pos.x + SinDeg(rotModel.y) * 10.0f;
		effPos.y = pos.y + 2.0f;
		effPos.z = pos.z + CosDeg(rotModel.y) * 10.0f;

		// エフェクトの設定
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.85f, 0.05f, 0.65f, 0.50f),
			XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f),
			XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
			XMFLOAT2(5.0f, 5.0f), 20);
	}

	//static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	//double dNowTime = clock() / double(CLOCKS_PER_SEC);
	//double dSlice = dNowTime - dLastTime;
	//dLastTime = dNowTime;
	//m_animTime += dSlice;
	//if (m_animTime >= myModel.GetAnimDuration(0)) {
	//	m_animTime = 0.0;
	//}


	// 弾発射
	if (GetKeyRepeat(VK_SPACE)) {
		Bullet::FireBullet(pos, XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLET_PLAYER);
	}
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Player::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	//myModel.SetAnimIndex(1);
	//myModel.SetAnimTime(m_animTime);
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効


	//PrintDebugProc("%d,%d\n", g_gameObject.m_mapIndex.x, g_gameObject.m_mapIndex.y);
	//PrintDebugProc("%f\n", moveVal.x);
	//PrintDebugProc("%f\n", moveVal.y);
	//PrintDebugProc("%f\n", moveVal.z);

	//DrawCollisionSphere(g_gameObject);

}

//====================================================================================
//
//				位置取得
//
//====================================================================================
XMFLOAT3& Player::GetPlayerPos() {
	return pos;
}

//====================================================================================
//
//				衝突判定
//
//====================================================================================
bool Player::CollisionPlayer(XMFLOAT3 pos, float radius, float damage) {
	Collision _collision;
	bool hit = _collision.CollisionSphere(pos, PLAYER_RADIUS, pos, radius);
	if (hit) {
		// 爆発開始
		int nExp = -1;
		if (damage > 0.0f) {
			nExp = StartExplosion(pos, XMFLOAT2(40.0f, 40.0f));
			// 
		} else {
			nExp = StartExplosion(pos, XMFLOAT2(20.0f, 20.0f));
		}
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
	}
	return hit;
}

bool Player::CollisionPlayer(GameObject collision) {
	GameObject* _GameObject;
	Collision _collision;

	_GameObject = this;

	bool isHit = _collision.CollisionSphere(*_GameObject, collision);

	if (isHit) {
		_GameObject = _collision.Push(pos, XMFLOAT3(PLAYER_RADIUS, PLAYER_RADIUS, PLAYER_RADIUS), moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));

		if (_GameObject == nullptr) {
			//　押し出しに失敗している
			return isHit;
		}

		pos = _GameObject->pos;
		moveVal = _GameObject->moveVal;
	}

	return isHit;
}

XMINT2 Player::SetPlayerIndex() {
	return mapIndex;
}