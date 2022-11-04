//************************************************************************************
// 
// シーン管理[Scene.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
//#include "HowToPlay.h"
#include "fade.h"


//-------------------- グローバル変数定義 --------------------
static Scene::eSCENE g_eScene = Scene::SCENE_NONE;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Scene::Scene() {

}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Scene::~Scene() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Scene::Init() {
	HRESULT hr = S_OK;

	SetScene(SCENE_TITLE);	// 最初はタイトル画面

	pTitle = new Title;
	pGame = new Game;

	InitFade();
}


//====================================================================================
//
//				終了
//
//====================================================================================
void Scene::Uninit() {
	SetScene(SCENE_NONE);	// 現在の画面を終了

	delete pGame;
	delete pTitle;

	UninitFade();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Scene::Update() {
	switch (g_eScene) {
	case SCENE_TITLE:	// タイトル画面
		pTitle->Update();
		break;
	//case SCENE_MODE:	// モード選択
	//	UpdateMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// 遊び方
	//	UpdateHowToPlay();
	//	break;
	case SCENE_GAME:	// ゲーム画面
		pGame->Update();
		break;
	case SCENE_RESULT:	// リザルト
		UpdateResult();
		break;
	//case SCENE_RANKING:	// ランキング
	//	UpdateRanking();
	//	break;
	default:
		break;
	}
	UpdateFade();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Scene::Draw() {
	switch (g_eScene) {
	case SCENE_TITLE:	// タイトル画面
		pTitle->Draw();
		break;
	//case SCENE_MODE:	// モード選択
	//	DrawMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// 遊び方
	//	DrawHowToPlay();
	//	break;
	case SCENE_GAME:	// ゲーム画面
		pGame->Draw();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	//case SCENE_RANKING:
	//	DrawRanking();
	//	break;
	default:
		break;
	}
	DrawFade();
}


//====================================================================================
//
//				切り替え
//
//====================================================================================
void Scene::SetScene(eSCENE eScene) {
	// 現在の画面を終了
	switch (g_eScene) {
	case SCENE_TITLE:		// タイトル画面
		pTitle->Uninit();
		break;
	//case SCENE_MODE:	// モード選択
	//	UninitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// 遊び方
	//	UninitHowToPlay();
	//	break;
	case SCENE_GAME:		// ゲーム画面
		pGame->Uninit();
		break;
	case SCENE_RESULT:		// リザルト
		UninitResult();
		break;
	//case SCENE_RANKING:		// ランキング
	//	UninitRanking();
	//	break;
	default:
		break;

	}
	// 画面を入れ替え
	g_eScene = eScene;
	// 次の画面を初期化
	switch (g_eScene) {
	case SCENE_TITLE:		// タイトル画面
		pTitle->Init();
		break;
	//case SCENE_MODE:	// モード選択
	//	InitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// 遊び方
	//	InitHowToPlay();
	//	break;
	case SCENE_GAME:		// ゲーム画面
		pGame->Init();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	//case SCENE_RANKING:
	//	InitRanking();
	//	break;
	default:
		break;
	}
}

//====================================================================================
//
//				シーン番号取得
//
//====================================================================================
Scene::eSCENE Scene::GetScene() {
	return g_eScene;
}
