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
Scene::eSCENE Scene::nowScene = Scene::SCENE_NONE;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Scene::Scene() {
	Init();
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Scene::~Scene() {
	Uninit();
}


//====================================================================================
//
//				初期化
//
//====================================================================================
void Scene::Init() {
	HRESULT hr = S_OK;

	// 最初はタイトル画面
	oldScene = nowScene = SCENE_TITLE;
	SetScene(nowScene);
	pNowScene = std::make_shared<Title>();

	//pTitle = std::make_shared<Title>();
	//pGame = std::make_shared<Game>();
	//pResult = std::make_shared<Result>();



	pFade = std::make_shared<Fade>();
}


//====================================================================================
//
//				終了
//
//====================================================================================
void Scene::Uninit() {
	SetScene(SCENE_NONE);	// 現在の画面を終了

	//pResult.reset();
	//pGame.reset();
	//pTitle.reset();

	pNowScene.reset();

	pFade.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Scene::Update() {
	//switch (nowScene) {
	//case SCENE_TITLE:	// タイトル画面
	//	pTitle->Update();
	//	break;
	//	//case SCENE_MODE:	// モード選択
	//	//	UpdateMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// 遊び方
	//	//	UpdateHowToPlay();
	//	//	break;
	//case SCENE_GAME:	// ゲーム画面
	//	pGame->Update();
	//	break;
	//case SCENE_RESULT:	// リザルト
	//	pResult->Update();
	//	break;
	//	//case SCENE_RANKING:	// ランキング
	//	//	UpdateRanking();
	//	//	break;
	//default:
	//	break;
	//}
	if (oldScene != nowScene) {
		ChangeScene();
	}

	pNowScene->Update();

	pFade->Update();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Scene::Draw() {
	//switch (nowScene) {
	//case SCENE_TITLE:	// タイトル画面
	//	pTitle->Draw();
	//	break;
	//	//case SCENE_MODE:	// モード選択
	//	//	DrawMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// 遊び方
	//	//	DrawHowToPlay();
	//	//	break;
	//case SCENE_GAME:	// ゲーム画面
	//	pGame->Draw();
	//	break;
	//case SCENE_RESULT:
	//	pResult->Draw();
	//	break;
	//	//case SCENE_RANKING:
	//	//	DrawRanking();
	//	//	break;
	//default:
	//	break;
	//}

	pNowScene->Draw();

	pFade->Draw();
}


//====================================================================================
//
//				次のシーンをセット
//
//====================================================================================
void Scene::SetScene(eSCENE eScene) {
	nowScene = eScene;
}

//====================================================================================
//
//				切り替え
//
//====================================================================================
void Scene::ChangeScene() {
	// 現在の画面を終了
	//switch (nowScene) {
	//case SCENE_TITLE:		// タイトル画面
	//	pTitle->Uninit();
	//	break;
	//	//case SCENE_MODE:	// モード選択
	//	//	UninitMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// 遊び方
	//	//	UninitHowToPlay();
	//	//	break;
	//case SCENE_GAME:		// ゲーム画面
	//	pGame->Uninit();
	//	break;
	//case SCENE_RESULT:		// リザルト
	//	pResult->Uninit();
	//	break;
	//	//case SCENE_RANKING:		// ランキング
	//	//	UninitRanking();
	//	//	break;
	//default:
	//	break;

	//}
	
	pNowScene->Uninit();

	// 画面を入れ替え
	//nowScene = nowScene;
	// 次の画面を初期化
	//switch (nowScene) {
	//case SCENE_TITLE:		// タイトル画面
	//	pTitle->Init();
	//	break;
	//	//case SCENE_MODE:	// モード選択
	//	//	InitMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// 遊び方
	//	//	InitHowToPlay();
	//	//	break;
	//case SCENE_GAME:		// ゲーム画面
	//	pGame->Init();
	//	break;
	//case SCENE_RESULT:
	//	pResult->Init();
	//	break;
	//	//case SCENE_RANKING:
	//	//	InitRanking();
	//	//	break;
	//default:
	//	break;
	//}
	switch (nowScene) {
	case SCENE_TITLE:		// タイトル画面
		pNowScene.reset();	// メモリ切り替えの為破棄
		pNowScene = std::make_shared<Title>();
		break;
	case SCENE_GAME:		// ゲーム画面
		pNowScene.reset();	// メモリ切り替えの為破棄
		pNowScene = std::make_shared<Game>();
		break;
	case SCENE_RESULT:
		pNowScene.reset();	// メモリ切り替えの為破棄
		pNowScene = std::make_shared<Result>();
		break;
	default:
		break;
	}
	oldScene = nowScene;
	pNowScene->Init();
}


//====================================================================================
//
//				シーン番号取得
//
//====================================================================================
Scene::eSCENE Scene::GetScene() {
	return nowScene;
}
