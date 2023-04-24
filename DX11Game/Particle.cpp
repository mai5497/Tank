//************************************************************************************
// 
// パーティクル[Particle.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Particle.h"
#include "input.h"
#include "Light.h"
#include "Camera.h"

#include "polygon.h"

//-------------------- 定数定義 --------------------
#define PATH_BGTEXTURE			(L"data/texture/effect000.jpg")

#define PARTICLE_POSX			(0)			// パーティクル表示基準位置
#define PARTICLE_POSY			(-200.0f)	
#define PARTICLE_SIZE			(100)		// パーティクルの最大の大きさ
#define PARTICLE_DISP			(0.2)		// パーティクルの散り具合

#define PARTICLE_TIME			(120)		// 消えるまでの時間


#define MATERIAL_DIFFUSE		XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

std::unique_ptr<Texture> Particle::pTexture;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Particle::Particle() {

}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Particle::~Particle() {

}

//====================================================================================
//
//				初期化
//
//====================================================================================
void Particle::Init() {
	//----- 変数初期化 -----
	isButtonOnce = false;


	//----- パーティクル初期化 -----
	ID3D11Device* pDevice = GetDevice();
	float sizeRate = 0.0f;

	for (int i = 0; i < PARTICLE_MAX; i++) {
		particles[i].status = 0;	// ステータスを初期化

		sizeRate = (rand() % 9) * 0.1 + 0.1;
		particles[i].size = PARTICLE_SIZE * sizeRate;

		particles[i].acceleration.x = (rand() % 9) * 0.001 + 0.001;
		particles[i].acceleration.y = (rand() % 9) * 0.001 + 0.001;

		particles[i].timer = PARTICLE_TIME;

		// オブジェクトの頂点配列を生成
		VERTEX_3D* pVertexWk = new VERTEX_3D[4];

		// 頂点配列の中身を埋める
		VERTEX_3D* pVtx = pVertexWk;

		// 頂点座標の設定
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
		pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
		pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
		pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

		// インデックス配列を生成
		int* pIndexWk = new int[4];

		// インデックス配列の中身を埋める
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		// 頂点バッファ生成
		pTexture = std::make_unique<Texture>();
		HRESULT  hr = pTexture->SetTexture(pDevice, PATH_BGTEXTURE);

		particles[i].color = XMFLOAT4((rand() % 9) * 0.1 + 0.1, (rand() % 9) * 0.1 + 0.1, (rand() % 9) * 0.1 + 0.1, 0.8f);
	}
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Particle::Uninit() {
	// テクスチャの開放
	pTexture->ReleaseTexture();
	pTexture.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Particle::Update() {
	//----- 変数初期化 -----
	float posx, posy;
	bool bBorn = false;

	//----- 入力があったら開始 -----
	if (GetKeyRelease(VK_SPACE)) {
		if (!isButtonOnce) {
			for (int i = 0; i < PARTICLE_MAX; i++) {
				particles[i].status = 1;
			}
		}
		isButtonOnce = true;	// 一度だけ処理したいのでtrueにして二回目以降はスルー
	}

	for (int i = 0; i < PARTICLE_MAX; i++) {
		switch (particles[i].status) {
			case 0:					// 待機状態
				break;
			case 1:
				particles[i].pos.x = PARTICLE_POSX;
				particles[i].pos.y = PARTICLE_POSY;
				posx = (i * PARTICLE_SIZE * (PARTICLE_MAX / 2)) - PARTICLE_SIZE * (PARTICLE_MAX / 4);
				particles[i].velocity.x = (posx)*particles[i].acceleration.x;
				posy = (i * PARTICLE_SIZE * (PARTICLE_MAX / 2)) - PARTICLE_SIZE * (PARTICLE_MAX / 4);
				particles[i].velocity.y = (posy)*particles[i].acceleration.y;
				particles[i].status = 2;

				break;
			case 2:
				particles[i].pos.x += particles[i].velocity.x;
				particles[i].pos.y += particles[i].velocity.y;

				particles[i].timer--;
				if (particles[i].timer < 1) {
					particles[i].status = 0;
				}
				break;
		}
	}
}


//====================================================================================
//
//				　描画
//
//====================================================================================
void Particle::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);		// 加算合成
	SetZWrite(false);	// 半透明描画はZバッファを更新しない(Zチェックは行う)

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (particles[i].status == 2) {
			SetPolygonSize(particles[i].size, particles[i].size);
			SetPolygonPos(particles[i].pos.x, particles[i].pos.y);
			SetPolygonTexture(pTexture->GetTexture());
			SetPolygonUV(0.0f, 0.0f);

			SetPolygonColor(particles[i].color.x, particles[i].color.y, particles[i].color.z);
			SetPolygonAlpha(particles[i].color.z);
			DrawPolygon(pDC);
		}
	}

	//----- 元に戻す -----
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}