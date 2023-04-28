//************************************************************************************
// 
// HPのUI[HPUI.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "HPUI.h"
#include "Player.h"
#include "Game.h"
#include "debugproc.h"

//-------------------- 定数定義 --------------------

#define PATH_EMPTY_TEXTURE	(L"data/texture/UI_EmptyHeart.png")
#define PATH_FULL_TEXTURE	(L"data/texture/UI_FullHeart.png")

#define UI_WIDTH			(145.0f / 2)
#define UI_HEIGHT			(145.0f / 2)

#define UI_BASE_POS_X		(SCREEN_WIDTH/2 - MAX_HP * UI_WIDTH + UI_WIDTH/2)
#define UI_BASE_POS_Y		(SCREEN_HEIGHT/2 - UI_HEIGHT + UI_HEIGHT/2)

std::unique_ptr<Texture> HPUI::pEmpty;
std::unique_ptr<Texture> HPUI::pFull;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
HPUI::HPUI() {

}



//====================================================================================
//
//				デストラクタ
//
//====================================================================================
HPUI::~HPUI() {

}



//====================================================================================
//
//				初期化
//
//====================================================================================
void HPUI::Init(Game* _pGameScene) {
	//----- 変数初期化 -----
	pGameScene = _pGameScene;	// ゲームシーンを保持

	playerHP = MAX_HP;	// HPの初期化

	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	pEmpty = std::make_unique<Texture>();
	pEmpty->SetTexture(pDevice, PATH_EMPTY_TEXTURE);

	pFull = std::make_unique<Texture>();
	pFull->SetTexture(pDevice, PATH_FULL_TEXTURE);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void HPUI::Uninit() {
	//----- HPUI解放 -----
	pFull->ReleaseTexture();
	pFull.reset();

	pEmpty->ReleaseTexture();
	pEmpty.reset();	
}

//====================================================================================
//
//				更新
//
//====================================================================================
void HPUI::Update() {
	// HPの取得
	playerHP = pGameScene->GetPlayerHP();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void HPUI::Draw() {
	SetBlendState(BS_ALPHABLEND);
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < MAX_HP; i++) {
		if (i < MAX_HP - playerHP) {	// HPがあるかないかどっちを描画するかの判定
			SetPolygonTexture(pEmpty->GetTexture());
		} else {
			SetPolygonTexture(pFull->GetTexture());
		}
		
		SetPolygonPos(UI_BASE_POS_X + i * UI_WIDTH, UI_BASE_POS_Y);
		DrawPolygon(pDC);
	}

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);

}
