//************************************************************************************
// 
// 背景[bg.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "BG.h"
#include "Texture.h"

//-------------------- 定数定義 --------------------
#define PATH_TITLE_TEXTURE	L"data/texture/BG_Sky.jpg"
#define PATH_SELECT_TEXTURE	L"data/texture/BG_Select.png"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

std::unique_ptr<Texture> BG::pSkyBG;
std::unique_ptr<Texture> BG::pSelectBG;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
BG::BG() {

}



//====================================================================================
//
//				デストラクタ
//
//====================================================================================
BG::~BG() {

}



//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT BG::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	pSkyBG = std::make_unique<Texture>();
	hr = pSkyBG->SetTexture(pDevice, PATH_TITLE_TEXTURE);

	pSelectBG = std::make_unique<Texture>();
	hr = pSelectBG->SetTexture(pDevice, PATH_SELECT_TEXTURE);

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void BG::Uninit() {
	// 背景テクスチャ解放
	pSelectBG->ReleaseTexture();
	pSelectBG.reset();

	pSkyBG->ReleaseTexture();
	pSkyBG.reset();

}

//====================================================================================
//
//				描画
//
//====================================================================================
void BG::Draw(eSCENE _nowScene) {
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	if (_nowScene == SCENE_TITLE || _nowScene == SCENE_GAME) {
		SetPolygonTexture(pSkyBG->GetTexture());
	} else if (_nowScene == SCENE_MODESELECT || _nowScene == SCENE_STAGESUMMARY || _nowScene == SCENE_TUTORIAL || _nowScene == SCENE_RESULT) {
		SetPolygonTexture(pSelectBG->GetTexture());
	}
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
}
