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
#include "Sound.h"
#include "Fade.h"
#include "TitleLogo.h"
#include "PressSpace.h"
#include "BG.h"
#include "meshfield.h"
#include "Particle.h"

//-------------------- 定数定義 --------------------
#define TITLE_POS_X			(0.0f)
#define TITLE_POS_Y			(0.0f)
#define TITLE_WIDTH			(SCREEN_WIDTH)
#define TITLE_HEIGHT		(SCREEN_HEIGHT)
#define MOVIETIMER			(600)
#define FADETIMER			(120)

//-------------------- グローバル変数定義 --------------------
static LPCWSTR g_pszTexFName = {
	L"data/texture/title.png"
};

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Title::Title() {
	//Init();
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Title::~Title() {
	//Uninit();
}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Title::Init() {
	ID3D11Device *pDevice = GetDevice();

	fadeTimer = FADETIMER;
	isFade = false;
	// ロゴ初期化
	pLogo = std::make_unique<TitleLogo>();
	pLogo->Init();

	// プレススペース初期化
	pStart = std::make_unique<PressSpace>();
	pStart->Init();

	// パーティクル初期化
	pParticle = std::make_unique<Particle>();
	pParticle->Init();

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

	// パーティクル終了処理
	pParticle->Uninit();
	pParticle.reset();

	// プレススペース終了
	pStart->Uninit();
	pStart.reset();

	// ロゴの終了
	pLogo->Uninit();
	pLogo.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Title::Update() {
	//----- シーン遷移 -----
	if (!isFade) {	// 入力を一度だけ受け付ける
		// クリックまたは[Space]押下
		if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
			CSound::Play(SE_DECIDE);
			isFade = true;
		}
	}

	// シーン遷移するフェードを行うまで少し待つ
	if (isFade) {
		fadeTimer--;
		if (fadeTimer < 1) {
			//モード選択画面へ
			Fade::StartFadeOut(SCENE_MODESELECT);
			return;
		}
	}

	//----- UIの更新 -----
	resetTimer--;
	if (resetTimer > 0) {
		// ロゴの更新
		pLogo->Update();

		// プレススペースの更新
		pStart->Update();

		// パーティクル
		pParticle->Update();

	} else {	// 0になったらリセットしてもう一度動かす
		pLogo->Reset();

		resetTimer = MOVIETIMER;
	}
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Title::Draw() {

	// 背景の描画
	BG::Draw(SCENE_TITLE);

	// ロゴの描画
	pLogo->Draw();

	// プレススペースの描画
	pStart->Draw();

	// パーティクルの描画
	pParticle->Draw();
}