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

static ID3D11ShaderResourceView *g_pTexture;

//====================================================================================
//
//				コンストラクタ
//
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
	CreateTextureFromFile(pDevice, g_pszTexFName, &g_pTexture);
	//nowScene = Scene::SCENE_TITLE;

	timer = MOVIETIMER;
	fadeTimer = FADETIMER;
	isFade = false;
	// ロゴ初期化
	pLogo = std::make_unique<TitleLogo>();
	pLogo->Init();

	// プレススペース初期化
	pStart = std::make_unique<PressSpace>();
	pStart->Init();

	// 背景初期化
	pBG = std::make_unique<BG>();
	pBG->Init();

	// フィールド初期化
	pMeshField = std::make_unique<MeshField>();
	pMeshField->Init(8, 7, 80.0f, 80.0f);

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
	// 背景テクスチャ開放
	SAFE_RELEASE(g_pTexture);

	// パーティクル終了処理
	pParticle->Uninit();
	pParticle.reset();

	// フィールド終了処理
	pMeshField->Uninit();
	pMeshField.reset();

	// 背景終了
	pBG->Uninit();
	pBG.reset();

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
	//クリックまたは[Enter]押下
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
		CSound::Play(SE_DECIDE);
		isFade = true;
	}
	if (isFade) {
		fadeTimer--;
		if (fadeTimer < 1) {
			//モード選択画面へ
			//SetScene(SCENE_GAME);
			Fade::StartFadeOut(SCENE_GAME);
			return;
		}
	}
	timer--;
	if (timer > 0) {
		// ロゴの更新
		pLogo->Update();

		// プレススペースの更新
		pStart->Update();

		// フィールド更新
		pMeshField->Update();

		// パーティクル
		pParticle->Update();

	} else {
		pLogo->Reset();

		timer = MOVIETIMER;
	}
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Title::Draw() {
	//// Zバッファ無効(Zチェック無&Z更新無)
	//SetZBuffer(false);

	//ID3D11DeviceContext *pDC = GetDeviceContext();
	//SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	//SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	//SetPolygonTexture(g_pTexture);
	//SetPolygonUV(0.0f, 0.0f);

	//DrawPolygon(pDC);


	// 背景の描画
	pBG->Draw();

	// フィールド描画
	pMeshField->Draw();

	// ロゴの描画
	pLogo->Draw();

	// プレススペースの描画
	pStart->Draw();

	// パーティクルの描画
	pParticle->Draw();
}