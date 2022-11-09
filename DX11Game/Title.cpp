//************************************************************************************
// 
// タイトル[Title.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "fade.h"
#include "Sound.h"

//-------------------- 定数定義 --------------------
#define TITLE_POS_X			0.0f
#define TITLE_POS_Y			0.0f
#define TITLE_WIDTH			SCREEN_WIDTH
#define TITLE_HEIGHT		SCREEN_HEIGHT

//-------------------- グローバル変数定義 --------------------
static LPCWSTR g_pszTexFName = {
	L"data/texture/title.png"
};

static ID3D11ShaderResourceView *g_pTexture;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Title::Title() {
	Init();
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Title::~Title() {
	Uninit();
}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Title::Init() {
	ID3D11Device *pDevice = GetDevice();
	CreateTextureFromFile(pDevice, g_pszTexFName, &g_pTexture);
	nowScene = Scene::SCENE_TITLE;

	// BGM再生
	CSound::Play(BGM_TITLE);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Title::Uninit() {

	// BGM再生停止
	CSound::Stop(BGM_TITLE);
	// 背景テクスチャ開放
	SAFE_RELEASE(g_pTexture);
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Title::Update() {
	//クリックまたは[Enter]押下
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
		CSound::Play(SE_DECIDE);
		//モード選択画面へ
		//SetScene(SCENE_GAME);
		Fade::StartFadeOut(Scene::SCENE_GAME);
		return;
	}
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Title::Draw() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);

	DrawPolygon(pDC);
}