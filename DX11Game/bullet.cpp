//************************************************************************************
// 
// 弾[bullet.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include "Bullet.h"
#include "Camera.h"
#include "shadow.h"
#include "Texture.h"
#include "Light.h"
#include "explosion.h"
#include "smoke.h"
#include "effect.h"
#include "debugproc.h"
#include "GameObjManager.h"
#include "Sound.h"

//-------------------- 定数定義 --------------------
#define	PLAYERBULLET_FILENAME	(L"data/texture/bullet_p.png")	// テクスチャ ファイル名
#define	ENEMYBULLET_FILENAME	(L"data/texture/bullet_e.png")	// テクスチャ ファイル名

#define M_DIFFUSE				XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER					(1.0f)
#define M_AMBIENT				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BULLET				(100)

#define BULLET_SPEED			(7.5f)
#define EBULLET_SPEED			(5.5f)
#define EMISSILE_SPEED			(9.0f)
#define BULLET_RADIUS			(10.0f)
#define BULLET_STRENGTH			(1.0f)
#define BULLET_GRAVITY			(0.95f)

#define MIN_FIELD_X				(-640.0f)
#define MAX_FIELD_X				(640.0f)

#define MIN_FIELD_Y				(10.0f)

#define MIN_FIELD_Z				(-640.0f)
#define MAX_FIELD_Z				(640.0f)

MESH Bullet::mesh_p;					// メッシュ情報
MESH Bullet::mesh_e;					// メッシュ情報
MATERIAL Bullet::material_p;			// マテリアル
MATERIAL Bullet::material_e;			// マテリアル


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

	//----- プレイヤーの弾初期化 -----
	// 頂点情報の作成
	MakeVertexBullet(pDevice, BULLET_PLAYER);

	// マテリアルの設定
	material_p.Diffuse = M_DIFFUSE;
	material_p.Ambient = M_AMBIENT;
	material_p.Specular = M_SPECULAR;
	material_p.Power = M_POWER;
	material_p.Emissive = M_EMISSIVE;
	mesh_p.pMaterial = &material_p;

	// テクスチャの読み込み
	std::unique_ptr<Texture> pTexture_p = std::make_unique<Texture>();
	HRESULT isLoad_p = pTexture_p->SetTexture(pDevice, PLAYERBULLET_FILENAME);
	if (FAILED(isLoad_p)) {
		MessageBox(NULL, _T("プレイヤーの弾テクスチャ読み込み失敗"), _T("error"), MB_OK);
	}
	mesh_p.pTexture = pTexture_p->GetTexture();
	pTexture_p.reset();

	XMStoreFloat4x4(&mesh_p.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_p.mtxWorld, XMMatrixIdentity());


	//----- 敵の弾初期化 -----
	// 頂点情報の作成
	MakeVertexBullet(pDevice, BULLET_ENEMY);

	// マテリアルの設定
	material_e.Diffuse = M_DIFFUSE;
	material_e.Ambient = M_AMBIENT;
	material_e.Specular = M_SPECULAR;
	material_e.Power = M_POWER;
	material_e.Emissive = M_EMISSIVE;
	mesh_e.pMaterial = &material_e;

	// テクスチャの読み込み
	std::unique_ptr<Texture> pTexture_e = std::make_unique<Texture>();
	HRESULT isLoad_e = pTexture_e->SetTexture(pDevice, ENEMYBULLET_FILENAME);
	if (FAILED(isLoad_e)) {
		MessageBox(NULL, _T("敵の弾テクスチャ読み込み失敗"), _T("error"), MB_OK);
	}
	mesh_e.pTexture = pTexture_e->GetTexture();
	pTexture_e.reset();

	XMStoreFloat4x4(&mesh_e.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_e.mtxWorld, XMMatrixIdentity());

	//----- 弾情報初期化 -----
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);
	use = false;
	isCollision = false;
	shadowNum = -1;
	collType = Collision::DYNAMIC;
	type = BULLETTYPE_NORMAL;
	myTag = BULLET_PLAYER;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Bullet::Uninit() {
	// 影の開放
	if (use) {
		use = false;
		ReleaseShadow(shadowNum);
		shadowNum = -1;
	}

	// メッシュの開放
	ReleaseMesh(&mesh_p);
	ReleaseMesh(&mesh_e);
}


//====================================================================================
//
//				更新
//
//====================================================================================
void Bullet::Update() {
	// 未使用ならスキップ
	if (!use) {
		return;
	}

	//----- 移動 -----
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	//----- 当たり判定 -----
	if (pos.x < MIN_FIELD_X || pos.x > MAX_FIELD_X ||
		pos.z < MIN_FIELD_Z || pos.z > MAX_FIELD_Z) {
		Destroy();
		return;
	}

	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (myTag == BULLET_PLAYER) {
				if (hitList[i].myTag == ENEMY) {	// プレイヤーとの当たり判定
					Destroy();
					return;
				} else if (hitList[i].myTag == WALL) {	// 壁との当たり判定
					Destroy();
					return;
				} else if (hitList[i].myTag == BULLET_ENEMY) {	// 弾との当たり判定
					Destroy();
					return;
				}
			} else if (myTag == BULLET_ENEMY) {
				if (hitList[i].myTag == PLAYER) {	// プレイヤーとの当たり判定
					Destroy();
					return;
				} else if (hitList[i].myTag == ENEMY) {	// 自分以外の敵との当たり判定
					if (hitList[i].gameObjNum != fireBulletObjNum) {
						Destroy();
					}
					return;
				} else if (hitList[i].myTag == WALL) {	// 壁との当たり判定
					Destroy();
					return;
				} else if (hitList[i].myTag == BULLET_PLAYER) {	// 弾との当たり判定
					Destroy();
					return;
				}
			}
		}
	}

	// 丸影移動
	MoveShadow(shadowNum, pos);

	// エフェクトの設定
	if (type == BULLETTYPE_NORMAL) {
		// 煙生成
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f));
	} else if (type == BULLETTYPE_MISSILE) {
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f), XMFLOAT4(1.0, 1.0f, 0.0f, 1.0f));
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
	// Zバッファ有効
	SetZBuffer(true);

	if (myTag == BULLET_PLAYER) {
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

	// Zバッファ無効
	SetZBuffer(false);
	SetBlendState(BS_NONE);		// αブレンディング無効
	CLight::Get()->SetEnable();	// 光源有効
}

//====================================================================================
//
//				発射
//
//====================================================================================
void Bullet::FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, ObjTag _tag, int objNum, EBulletType _type) {
	std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>();

	pBullet->Init();	// 初期化

	CSound::Play(SE_NORMALBULLET);

	// 方向ベクトルを正規化
	XMStoreFloat3(&_dir, XMVector3Normalize(XMLoadFloat3(&_dir)));
	if (_tag == BULLET_PLAYER) {
		pBullet->moveVal.x = _dir.x * BULLET_SPEED;
		pBullet->moveVal.y = _dir.y * BULLET_SPEED;
		pBullet->moveVal.z = _dir.z * BULLET_SPEED;
	} else {
		if (_type == BULLETTYPE_NORMAL) {
			pBullet->moveVal.x = _dir.x * EBULLET_SPEED;
			pBullet->moveVal.y = _dir.y * EBULLET_SPEED;
			pBullet->moveVal.z = _dir.z * EBULLET_SPEED;
		} else if (_type == BULLETTYPE_MISSILE) {
			pBullet->moveVal.x = _dir.x * EMISSILE_SPEED;
			pBullet->moveVal.y = _dir.y * EMISSILE_SPEED;
			pBullet->moveVal.z = _dir.z * EMISSILE_SPEED;
		}
	}

	pBullet->pos = _pos;
	pBullet->shadowNum = CreateShadow(_pos, BULLET_RADIUS);
	XMFLOAT4X4 _mtxWorld;
	XMStoreFloat4x4(&_mtxWorld,XMMatrixTranslation(pBullet->pos.x, pBullet->pos.y, pBullet->pos.z));
	pBullet->use = true;
	pBullet->type = _type;
	pBullet->isCollision = true;
	pBullet->myTag = _tag;
	pBullet->fireBulletObjNum = objNum;	// 撃ったオブジェクトの番号を保存する

	pBullet->gameObjNum = GameObjManager::AddList(pBullet, false);	// 上ですでに初期化はしているので初期化は必要ない

	pBullet.reset();
}

//====================================================================================
//
//				頂点情報の作成
//
//====================================================================================
void Bullet::MakeVertexBullet(ID3D11Device* pDevice, ObjTag _objType) {
	// 一時的な頂点配列を生成
	MESH* pWork;
	if (_objType == BULLET_PLAYER) {
		pWork = &mesh_p;
	} else {
		pWork = &mesh_e;
	}

	pWork->nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[pWork->nNumVertex];

	pWork->primitiveType = PT_TRIANGLESTRIP;

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
	pWork->nNumIndex = 4;
	int* pIndexWk = new int[4];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ/インデックス バッファ生成
	MakeMeshVertex(pDevice, pWork, pVertexWk, pIndexWk);

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

	CSound::Play(SE_DAMAGE);
	
	// 丸影解放
	ReleaseShadow(shadowNum);
	shadowNum = -1;

	// 爆発開始
	pos.x -= moveVal.x;
	pos.y -= moveVal.y;
	pos.z -= moveVal.z;
	int nExp = StartExplosion(pos, XMFLOAT2(20.0f, 20.0f));

	isCollision = false;

	GameObjManager::DelList(gameObjNum, false);
}

