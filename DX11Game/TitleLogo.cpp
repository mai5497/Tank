//************************************************************************************
// 
// タイトルロゴ[TitleLogo.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "TitleLogo.h"

//-------------------- 定数定義 --------------------
#define TEXTURE_PATH			(L"data/texture/UI_TitleLogo.png")
#define TEXTURE_WIDTH			(900)
#define TEXTURE_HEIGHT			(150)

#define TEXTURE_POSX_BASE		(-375.0f)


#define SPRING_DEFAULT_LENGTH	(2.0f)			// バネの自然長
#define SPRING_COEFFICIENT		(1.0f)			// バネ係数
#define SPRING_MASS				(10.0f)			// バネの質量
#define ATT_COEFFICIENT			(1.0f)			// 減衰係数(Attenuation)


#define F_ZERO(v)			v.x=v.y=0.0f
#define F_SET(v,px,py)		v.x=px;v.y=py
#define F_SQRT(v)			sqrtf(v.x*v.x+v.y*v.y)

std::unique_ptr<Texture> TitleLogo::pTexture;
LPCWSTR TitleLogo::pszTexFName;


//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
TitleLogo::TitleLogo() {

}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
TitleLogo::~TitleLogo() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void TitleLogo::Init() {
	pszTexFName = TEXTURE_PATH;

	ID3D11Device* pDevice = GetDevice();

	pTexture = std::make_unique<Texture>();
	pTexture->SetTexture(pDevice, pszTexFName);

	for (int j = 0; j < STRING_TOTAL; j++) {
		float massRate = (rand() % 10) * 0.1 + 0.1f;
		for (int i = 0; i < SPRING_TOTAL; i++) {
			springs[j][i].status = 0;		// ばねの初期化
			springs[j][i].mass = SPRING_MASS * massRate;
		}
	}

}

//====================================================================================
//
//				リセット
//
//====================================================================================
void TitleLogo::Reset() {
	for (int j = 0; j < STRING_TOTAL; j++) {
		for (int i = 0; i < SPRING_TOTAL; i++) {
			springs[j][i].status = 0;		// ばねの初期化
		}
	}
}

//====================================================================================
//
//				終了
//
//====================================================================================
void TitleLogo::Uninit() {
	// テクスチャ開放
	pTexture->ReleaseTexture();
	pTexture.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void TitleLogo::Update() {
	//----- 変数初期化 -----
	float length;			// バネの長さ
	float posx;				// 初期化用座標計算
	XMFLOAT2 vec_up;		// 上側の力
	XMFLOAT2 vec_down;		// 下側の力
	XMFLOAT2 resistance;	// 抵抗力
	float now_tension;		// 弾性力の大きさ
	int	i,j;

	//----- ばねの処理 -----
	for (j = 0; j < STRING_TOTAL; j++) {
		for (i = 0; i < SPRING_TOTAL; i++) {
			switch (springs[j][i].status) {
			case 0:			// 初期化
				posx = TEXTURE_POSX_BASE + j * (TEXTURE_WIDTH / STRING_TOTAL);

				F_SET(springs[j][i].position, posx, -SPRING_DEFAULT_LENGTH * i + TEXTURE_HEIGHT);	// 座標
				F_ZERO(springs[j][i].velocity);														// 速度
				F_ZERO(springs[j][i].acceleration);													// 加速度
				F_ZERO(springs[j][i].tension_up);													// 弾性力
				F_ZERO(springs[j][i].tension_down);													// 弾性力
				F_ZERO(springs[j][i].resultant);													// 合力
				F_SET(springs[j][i].gravity, 0.0f, springs[j][i].mass * -0.98f);					// 重力
				springs[j][i].onLock = (i == 0);													// 上端はロック
				springs[j][i].status = 1;
				break;
			case 1:
				// １：上側に引っ張られる力を求める
				// 上側に接続されているばねについて弾力性を求める
				if (i == 0) {
					springs[j][i].tension_up.x = 0.0f;	// 最上部なので上からの力は０
					springs[j][i].tension_up.y = 0.0f;
				} else {
					vec_up.x = springs[j][i - 1].position.x - springs[j][i].position.x;	// 上方向のベクトルを求める
					vec_up.y = springs[j][i - 1].position.y - springs[j][i].position.y;	// 上方向のベクトルを求める
					length = sqrt(vec_up.x * vec_up.x + vec_up.y * vec_up.y);
					if (length == 0) {
						springs[j][i].tension_up.x = 0.0f;	// 長さが０なら力も０
						springs[j][i].tension_up.y = 0.0f;
					} else {
						now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);	// 定数と自然長から計算
						springs[j][i].tension_up.x = vec_up.x * (now_tension / length);	// 上方向の力を示すベクトルを計算
						springs[j][i].tension_up.y = vec_up.y * (now_tension / length);
					}
				}


				// ２：下側に引っ張られる力を求める
				if (i == SPRING_TOTAL - 1) {
					springs[j][i].tension_down.x = 0.0f;	// 最下部なので下からの力は０
					springs[j][i].tension_down.y = 0.0f;
				} else {
					vec_down.x = springs[j][i + 1].position.x - springs[j][i].position.x;	// 下方向のベクトルを求める
					vec_down.y = springs[j][i + 1].position.y - springs[j][i].position.y;
					length = sqrt(vec_down.x * vec_down.x + vec_down.y * vec_down.y);
					if (length == 0) {
						springs[j][i].tension_down.x = 0.0f;	// 長さが０なら力も０
						springs[j][i].tension_down.y = 0.0f;
					} else {
						now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);	// 定数と自然長から計算
						springs[j][i].tension_down.x = vec_down.x * (now_tension / length);	// 下方向の力を示すベクトルを計算
						springs[j][i].tension_down.y = vec_down.y * (now_tension / length);
					}
				}

				// ３：重力を求める
				springs[j][i].gravity.x = 0.0f;
				springs[j][i].gravity.y = springs[j][i].mass * -0.98f;	// ばね定数と重力加速度

				// ４：抵抗力を求める
				resistance.x = -springs[j][i].velocity.x * ATT_COEFFICIENT;	// 速度の逆方向に抵抗力定数をかける
				resistance.y = -springs[j][i].velocity.y * ATT_COEFFICIENT;

				// ５：１から４で求めた力を合成して合力を求める
				springs[j][i].resultant.x = springs[j][i].gravity.x + springs[j][i].tension_up.x + springs[j][i].tension_down.x + resistance.x;
				springs[j][i].resultant.y = springs[j][i].gravity.y + springs[j][i].tension_up.y + springs[j][i].tension_down.y + resistance.y;

				if (springs[j][i].onLock == false) {
					// ６：合力と質量から加速度を求める
					springs[j][i].acceleration.x = springs[j][i].resultant.x / springs[j][i].mass;
					springs[j][i].acceleration.y = springs[j][i].resultant.y / springs[j][i].mass;

					// ７：加速度から速度を求める
					springs[j][i].velocity.x += springs[j][i].acceleration.x;
					springs[j][i].velocity.y += springs[j][i].acceleration.y;

					// ８：速度から位置を求める
					springs[j][i].position.x += springs[j][i].velocity.x;
					springs[j][i].position.y += springs[j][i].velocity.y;
				}

				break;
			}
		}
	}

}



//====================================================================================
//
//				描画
//
//===================================================================================
void TitleLogo::Draw() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	SetPolygonFrameSize(1.0f / STRING_TOTAL, 1.0f);

	for (int i = 0; i < STRING_TOTAL; i++) {
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetPolygonSize(TEXTURE_WIDTH/STRING_TOTAL, TEXTURE_HEIGHT);
		SetPolygonPos(springs[i][SPRING_TOTAL - 1].position.x, springs[i][SPRING_TOTAL - 1].position.y);
		SetPolygonTexture(pTexture->GetTexture());
		SetPolygonUV((i % STRING_TOTAL) / (float)STRING_TOTAL, 0.0f);

		DrawPolygon(pDC);
	}

	//----- 元に戻す -----
	// Zバッファ無効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	SetPolygonFrameSize(1.0f , 1.0f);

}