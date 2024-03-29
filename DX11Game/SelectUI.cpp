//************************************************************************************
// 
// モード選択のUI[SelectUI.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "SelectUI.h"

#include "Texture.h"
#include "input.h"
#include "Sound.h"

//-------------------- 定数 --------------------
#define PATH_UI_TEXTURE		(L"data/texture/UI_Select.png")
#define PATH_BOX_TEXTURE	(L"data/texture/UI_SelectBox.png")

#define UI_WIDTH			(400)
#define UI_HEIGHT			(240)
#define UI_SPLIT_Y			(2)

#define BOX_WIDTH			(445)
#define BOX_HEIGHT			(165)

std::unique_ptr<Texture> SelectUI::pUI;
std::unique_ptr<Texture> SelectUI::pBox;


//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
SelectUI::SelectUI() {

}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
SelectUI::~SelectUI() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT SelectUI::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//----- テクスチャ読み込み -----
	pUI = std::make_unique<Texture>();
	hr = pUI->SetTexture(pDevice, PATH_UI_TEXTURE);

	pBox = std::make_unique<Texture>();
	hr = pBox->SetTexture(pDevice, PATH_BOX_TEXTURE);

	selectNum = (int)START;

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void SelectUI::Uninit() {
	// テクスチャ開放
	pBox->ReleaseTexture();
	pBox.reset();

	pUI->ReleaseTexture();
	pUI.reset();
}


//====================================================================================
//
//				更新
//
//====================================================================================
int SelectUI::Update() {
	//----- 選択と範囲外処理 -----
	if (GetKeyRelease(VK_W) || GetKeyRelease(VK_UP)) {
		selectNum--;
		if (selectNum < START) {
			selectNum = START;
		}
		// SE再生
		CSound::Play(SE_SELECT);
	}
	if (GetKeyRelease(VK_S) || GetKeyRelease(VK_DOWN)) {
		selectNum++;
		if (selectNum > TUTORIAL) {
			selectNum = TUTORIAL;
		}
		// SE再生
		CSound::Play(SE_SELECT);
	}

	// 選択された数字を返すことで決定キーが押されたときにそのシーンへ飛ぶ
	return selectNum;
}


//====================================================================================
//
//			描画
//
//====================================================================================
void SelectUI::Draw() {
	SetBlendState(BS_ALPHABLEND);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	//----- BOX -----
	SetPolygonSize(BOX_WIDTH, BOX_HEIGHT);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(pBox->GetTexture());
	SetPolygonPos(0.0f, 0.0f + (UI_HEIGHT / 2) - (UI_HEIGHT / 2 / 2 + 85)*selectNum);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);


	//----- UI -----
	SetPolygonSize(UI_WIDTH, UI_HEIGHT/2);
	SetPolygonFrameSize(1.0f, 1.0f / UI_SPLIT_Y);

	SetPolygonTexture(pUI->GetTexture());
	for (int i = 0; i < UI_SPLIT_Y; i++) {
		SetPolygonPos(0.0f, 0.0f + (UI_HEIGHT/2) - (UI_HEIGHT/2/2+85)*i);
		SetPolygonUV(0.0f, i / (float)UI_SPLIT_Y);
		DrawPolygon(pDC);
	}



	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}