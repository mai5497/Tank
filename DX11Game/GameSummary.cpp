#include "GameSummary.h"

#include "Texture.h"
#include "input.h"
#include "fade.h"

#include "bg.h"


#define PATH_UI_TEXTURE	L"data/texture/uiSousa.png"

#define UI_WIDTH		(615)
#define UI_HEIGHT		(401)

#define SCENE_CHANGE_TIME	(180)


// 静的メンバ変数初期化
std::unique_ptr<Texture> GameSummary::pBG;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
GameSummary::GameSummary() {

}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
GameSummary::~GameSummary() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void GameSummary::Init() {
	ID3D11Device* pDevice = GetDevice();

	//pBG = std::make_unique<Texture>();
	//pBG->SetTexture(pDevice, PATH_UI_TEXTURE);

	sceneChangeTimer = SCENE_CHANGE_TIME;
}


//====================================================================================
//
//				終了
//
//====================================================================================
void GameSummary::Uninit() {
	//pBG->ReleaseTexture();
	//pBG.reset();
}


//====================================================================================
//
//				更新
//
//====================================================================================
void GameSummary::Update() {
	sceneChangeTimer--;
	if (sceneChangeTimer < 0) {
		Fade::StartFadeOut(SCENE_GAME);
		sceneChangeTimer = SCENE_CHANGE_TIME;
	}
}


//====================================================================================
//
//				描画
//
//====================================================================================
void GameSummary::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	BG::Draw(SCENE_STAGESUMMARY);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	//SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	//SetPolygonFrameSize(1.0f, 1.0f);
	//SetPolygonTexture(pBG->GetTexture());
	//SetPolygonPos(0.0f, 0.0f);
	//SetPolygonUV(0.0f, 0.0f);
	//DrawPolygon(pDC);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}