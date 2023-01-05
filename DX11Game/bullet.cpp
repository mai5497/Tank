//************************************************************************************
// 
// 弾[bullet.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include "bullet.h"
#include "Camera.h"
#include "shadow.h"
#include "Texture.h"
#include "Light.h"
#include "explosion.h"
#include "smoke.h"
#include "effect.h"
#include "debugproc.h"
#include "GameObjManager.h"

//-------------------- 定数定義 --------------------
#define	PLAYERBULLET_FILENAME	L"data/texture/bullet_p.png"	// テクスチャ ファイル名
#define	ENEMYBULLET_FILENAME	L"data/texture/bullet_e.png"	// テクスチャ ファイル名

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BULLET			100

#define BULLET_SPEED		7.5f
#define EBULLET_SPEED		5.5f
#define BULLET_RADIUS		10.0f
#define BULLET_STRENGTH		1.0f
#define BULLET_GRAVITY		0.95f

#define MIN_FIELD_X			-640.0f
#define MAX_FIELD_X			640.0f

#define MIN_FIELD_Y			(10.0f)

#define MIN_FIELD_Z			-640.0f
#define MAX_FIELD_Z			640.0f

MESH Bullet::mesh_p;					// メッシュ情報
MESH Bullet::mesh_e;					// メッシュ情報
MATERIAL Bullet::material;				// マテリアル


//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Bullet::Bullet() {

}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Bullet::~Bullet() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Bullet::Init(void) {
	ID3D11Device* pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// マテリアルの設定
	material.Diffuse = M_DIFFUSE;
	material.Ambient = M_AMBIENT;
	material.Specular = M_SPECULAR;
	material.Power = M_POWER;
	material.Emissive = M_EMISSIVE;
	mesh_p.pMaterial = &material;
	mesh_e.pMaterial = &material;

	// テクスチャの読み込み
	CreateTextureFromFile(pDevice, PLAYERBULLET_FILENAME, &mesh_p.pTexture);
	CreateTextureFromFile(pDevice, ENEMYBULLET_FILENAME, &mesh_e.pTexture);

	XMStoreFloat4x4(&mesh_p.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_e.mtxTexture, XMMatrixIdentity());

	// ワールド マトリックス初期化
	XMStoreFloat4x4(&mesh_p.mtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_e.mtxWorld, XMMatrixIdentity());

	// 弾情報初期化
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	collSize = XMFLOAT3(25.0f, 25.0f, 25.0f);
	use = false;
	isCollision = false;
	shadowNum = -1;
	collType = Collision::DYNAMIC;
	type = BULLETTYPE_PLAYER;
	myTag = BULLET_PLAYER;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Bullet::Uninit() {
	if (use) {
		use = false;
		ReleaseShadow(shadowNum);
		shadowNum = -1;
	}
	ReleaseMesh(&mesh_p);
	ReleaseMesh(&mesh_e);
}


//====================================================================================
//
//				更新
//
//====================================================================================
void Bullet::Update() {
	//for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		// 未使用ならスキップ
	if (!use) {
		return;
	}
	// 位置を更新
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.y *= BULLET_GRAVITY;
	pos.z += moveVal.z;
	// 範囲チェック
	if (pos.x < MIN_FIELD_X || pos.x > MAX_FIELD_X ||
		//pBullet->pos.y < MIN_FIELD_Y ||
		pos.z < MIN_FIELD_Z || pos.z > MAX_FIELD_Z) {

		Destroy();

		return;
	}
	if (type == BULLETTYPE_PLAYER) {
		if (hitList.size() > 0) {
			for (int i = 0; i < hitList.size(); i++) {
				if (hitList[i] == ENEMY) {	// プレイヤーとの当たり判定
					Destroy();
				} else if (hitList[i] == WALL) {	// 壁との当たり判定
					Destroy();
				} else if (hitList[i] == BULLET_ENEMY) {	// 弾との当たり判定
					Destroy();
				}
			}
		}
	} else if (type == BULLETTYPE_ENEMY) {
		if (hitList.size() > 0) {
			for (int i = 0; i < hitList.size(); i++) {
				if (hitList[i] == PLAYER) {	// プレイヤーとの当たり判定
					Destroy();
				}else if(hitList[i] == ENEMY) {	// 自分以外の敵との当たり判定
					Destroy();
				}else if (hitList[i] == WALL) {	// 壁との当たり判定
					Destroy();
				} else if (hitList[i] == BULLET_PLAYER) {	// 弾との当たり判定
					Destroy();
				}
			}
		}


	}

	// 丸影移動
	MoveShadow(shadowNum, pos);
	// エフェクトの設定
	if (type == BULLETTYPE_PLAYER) {
		// 煙生成
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f));
	} else {
		// 煙生成
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f));

	}

}

//====================================================================================
//
//				描画
//
//====================================================================================
void Bullet::Draw() {
	// 未使用ならスキップ
	if (!use) {
		return;
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// 光源無効
	SetBlendState(BS_ALPHABLEND);	// αブレンディング有効
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();

	if (type == BULLETTYPE_PLAYER) {
		// ビュー行列の回転成分の転置行列を設定
		mesh_p.mtxWorld._11 = mView._11;
		mesh_p.mtxWorld._12 = mView._21;
		mesh_p.mtxWorld._13 = mView._31;
		mesh_p.mtxWorld._14 = 0.0f;
		mesh_p.mtxWorld._21 = mView._12;
		mesh_p.mtxWorld._22 = mView._22;
		mesh_p.mtxWorld._23 = mView._32;
		mesh_p.mtxWorld._24 = 0.0f;
		mesh_p.mtxWorld._31 = mView._13;
		mesh_p.mtxWorld._32 = mView._23;
		mesh_p.mtxWorld._33 = mView._33;
		mesh_p.mtxWorld._34 = 0.0f;
		// 位置を反映
		mesh_p.mtxWorld._41 = pos.x;
		mesh_p.mtxWorld._42 = pos.y;
		mesh_p.mtxWorld._43 = pos.z;
		mesh_p.mtxWorld._44 = 1.0f;
		// 描画
		DrawMesh(pDC, &mesh_p);

	} else {
		// ビュー行列の回転成分の転置行列を設定
		mesh_e.mtxWorld._11 = mView._11;
		mesh_e.mtxWorld._12 = mView._21;
		mesh_e.mtxWorld._13 = mView._31;
		mesh_e.mtxWorld._14 = 0.0f;
		mesh_e.mtxWorld._21 = mView._12;
		mesh_e.mtxWorld._22 = mView._22;
		mesh_e.mtxWorld._23 = mView._32;
		mesh_e.mtxWorld._24 = 0.0f;
		mesh_e.mtxWorld._31 = mView._13;
		mesh_e.mtxWorld._32 = mView._23;
		mesh_e.mtxWorld._33 = mView._33;
		mesh_e.mtxWorld._34 = 0.0f;
		// 位置を反映
		mesh_e.mtxWorld._41 = pos.x;
		mesh_e.mtxWorld._42 = pos.y;
		mesh_e.mtxWorld._43 = pos.z;
		mesh_e.mtxWorld._44 = 1.0f;
		// 描画
		DrawMesh(pDC, &mesh_e);
	}


	SetBlendState(BS_NONE);		// αブレンディング無効
	CLight::Get()->SetEnable();	// 光源有効
}

//====================================================================================
//
//				発射
//
//====================================================================================
void Bullet::FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, EBulletType _type) {
	std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>();

	pBullet->Init();

	pBullet->pos = _pos;
	// 方向ベクトルを正規化
	XMStoreFloat3(&_dir, XMVector3Normalize(XMLoadFloat3(&_dir)));
	if (_type == BULLETTYPE_PLAYER) {
		pBullet->moveVal.x = _dir.x * BULLET_SPEED;
		pBullet->moveVal.y = _dir.y * BULLET_SPEED;
		pBullet->moveVal.z = _dir.z * BULLET_SPEED;
	} else {
		pBullet->moveVal.x = _dir.x * EBULLET_SPEED;
		pBullet->moveVal.y = _dir.y * EBULLET_SPEED;
		pBullet->moveVal.z = _dir.z * EBULLET_SPEED;
	}
	pBullet->shadowNum = CreateShadow(_pos, BULLET_RADIUS);
	XMFLOAT4X4 _mtxWorld;
	XMStoreFloat4x4(&_mtxWorld,
		XMMatrixTranslation(pBullet->pos.x, pBullet->pos.y, pBullet->pos.z));
	pBullet->use = true;
	pBullet->type = _type;
	pBullet->isCollision = true;

	pBullet->gameObjNum = GameObjManager::AddList(pBullet, false);	// 上ですでに初期化はしているので初期化は必要ない

	pBullet.reset();
}

//====================================================================================
//
//				頂点情報の作成
//
//====================================================================================
void Bullet::MakeVertexBullet(ID3D11Device* pDevice) {
	// 一時的な頂点配列を生成
	mesh_p.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[mesh_p.nNumVertex];

	// 頂点配列の中身を埋める
	pVertexWk[0].vtx = XMFLOAT3(-BULLET_RADIUS, BULLET_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(BULLET_RADIUS, BULLET_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// 一時的なインデックス配列を生成
	mesh_p.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ/インデックス バッファ生成
	MakeMeshVertex(pDevice, &mesh_p, pVertexWk, pIndexWk);

	// 一時的な頂点配列/インデックス配列を解放
	delete[] pIndexWk;
	delete[] pVertexWk;
}


//====================================================================================
//
//				消える
//
//====================================================================================
void Bullet::Destroy() {
	use = false;
	// 丸影解放
	ReleaseShadow(shadowNum);
	shadowNum = -1;
	// 爆発開始
	pos.x -= moveVal.x;
	pos.y -= moveVal.y;
	pos.z -= moveVal.z;
	int nExp = StartExplosion(pos, XMFLOAT2(20.0f, 20.0f));

	isCollision = false;

	GameObjManager::DelList(gameObjNum);
}

