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
#define PATH_BGTEXTURE	L"data/texture/sky001.jpg"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

std::unique_ptr<Texture> BG::pBG;

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
HRESULT BG::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	pBG = std::make_unique<Texture>();
	hr =  pBG->SetTexture(pDevice, PATH_BGTEXTURE);

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void BG::Uninit()
{
	// 背景テクスチャ解放
	pBG->ReleaseTexture();
	pBG.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void BG::Update()
{
	// 何もしない
}

//====================================================================================
//
//				描画
//
//====================================================================================
void BG::Draw()
{
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(pBG->GetTexture());
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
}
