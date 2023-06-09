#include "Tutorial.h"

#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "Sound.h"

#include "bg.h"


#define PATH_UI_TEXTURE	L"data/texture/UI_Sousa.png"

#define UI_WIDTH		(615)
#define UI_HEIGHT		(401)


// 静的メンバ変数初期化
std::unique_ptr<Texture> Tutorial::pUI;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Tutorial::Tutorial() {

}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Tutorial::~Tutorial() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Tutorial::Init() {
	ID3D11Device* pDevice = GetDevice();

	pUI = std::make_unique<Texture>();
	pUI->SetTexture(pDevice,PATH_UI_TEXTURE);
	
	// BGM再生
	CSound::Play(BGM_MODE);
}


//====================================================================================
//
//				終了
//
//====================================================================================
void Tutorial::Uninit() {
	// BGM再生停止
	CSound::Stop(BGM_MODE);

	pUI->ReleaseTexture();
	pUI.reset();
}


//====================================================================================
//
//				更新
//
//====================================================================================
void Tutorial::Update() {
	if (GetKeyRelease(VK_RETURN)) {
		Fade::StartFadeOut(SCENE_MODESELECT);
	}
}


//====================================================================================
//
//				描画
//
//====================================================================================
void Tutorial::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	BG::Draw(SCENE_TUTORIAL);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(pUI->GetTexture());
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}