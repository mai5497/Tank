//************************************************************************************
// 
// シーン管理[SceneManager.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
#include "SceneManager.h"
#include "Scene.h"

#include "Title.h"
#include "Select.h"
#include "Tutorial.h"
#include "GameSummary.h"
#include "Game.h"
#include "Result.h"

#include "Fade.h"

#include "Camera.h"

//-------------------- グローバル変数定義 --------------------
eSCENE SceneManager::nextScene = SCENE_NONE;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
SceneManager::SceneManager() {
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
SceneManager::~SceneManager() {
}

//====================================================================================
//
//				初期化
//
//====================================================================================
void SceneManager::Init() {
	stageNum = -1;	// ステージ番号は0から シーンはいるときに加算するので-1で初期化

	// 最初はタイトル画面
#if 0
	nowScene = nextScene = SCENE_TITLE;
	if(!pNowScene)pNowScene = std::make_shared<Title>();
#else
	stageNum++;
	nowScene = nextScene = SCENE_GAME;
	if (!pNowScene)pNowScene = std::make_shared<Game>(stageNum);
#endif
	pNowScene->Init();

	// フェードの初期化
	if(!pFade)pFade = std::make_shared<Fade>();
}

//====================================================================================
//
//				終了
//
//====================================================================================
void SceneManager::Uninit() {
	SetScene(SCENE_NONE);	// 現在の画面を終了

	//----- メモリの開放 -----
	if(pNowScene)pNowScene.reset();
	if(pFade)pFade.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void SceneManager::Update() {

	//----- シーン切り替え -----
	if (nowScene != nextScene) {
		ChangeScene();
	}


	//----- 各種更新処理 -----
	if (!pNowScene) {
		MessageBox(NULL, _T("シーン消失エラー\nSceneManager.cpp(79)"), _T("error"), MB_OK);
	}
	////----- カメラ更新 -----
	//CCamera::Get()->Update(nowScene);

	pNowScene->Update();

	pFade->Update();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void SceneManager::Draw() {	
	pNowScene->Draw();
	pFade->Draw();
}

//====================================================================================
//
//				次のシーンをセット
//
//====================================================================================
void SceneManager::SetScene(eSCENE eScene) {
	nextScene = eScene;	// 次のシーンとして格納しておく、Updateで切り替えの関数を呼ぶ
}

//====================================================================================
//
//				シーン番号取得
//
//====================================================================================
eSCENE SceneManager::GetScene() {
	return nowScene;
}


//====================================================================================
//
//				切り替え
//
//====================================================================================
void SceneManager::ChangeScene() {
	// 現在のシーンの終了処理
	pNowScene->Uninit();
	pNowScene.reset();	// メモリ切り替えの為破棄

	// カメラの初期化
	CCamera::Get()->Init();

	// 次のシーンによって格納するものをかえる
	switch (nextScene) {
	case SCENE_TITLE:		// タイトル画面
		pNowScene = std::make_shared<Title>();
		break;
	case SCENE_MODESELECT:	// セレクト画面
		pNowScene = std::make_shared<Select>();
		break;
	case SCENE_TUTORIAL:	// チュートリアル
		pNowScene = std::make_shared<Tutorial>();
		break;
	case SCENE_STAGESUMMARY:// ステージ概要
		stageNum++;
		pNowScene = std::make_shared<GameSummary>();
		break;
	case SCENE_GAME:		// ゲーム画面
		pNowScene = std::make_shared<Game>(stageNum);
		break;
	case SCENE_RESULT:		// リザルトシーン
		pNowScene = std::make_shared<Result>();
		break;
	default:
		MessageBox(NULL, _T("シーンの切り替えに失敗しました。\nSceneManager.cpp(159)"), _T("error"), MB_OK);
		break;
	}

	// 新しいメモリになったので初期化
	pNowScene->Init();

	// 現在のシーン番号を更新しておく
	nowScene = nextScene;
}
