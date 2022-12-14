//************************************************************************************
// 
// ゲーム管理[Game.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Game.h"
#include "Sound.h"
#include "shadow.h"
#include "effect.h"
#include "DwarfEffect.h"
#include "smoke.h"
#include "timer.h"
#include "input.h"

#include "BG.h"
#include "explosion.h"
#include "Meshfield.h"
#include "Fade.h"
#include "GameObjManager.h"

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Game::Game() {
	//Init();
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Game::~Game() {
	//Uninit();
}

//====================================================================================
//
//				初期化
//
//====================================================================================
void Game::Init() {
	// 丸影初期化
	InitShadow();

	// ゲームオブジェクト管理クラス初期化
	pGameObjects = std::make_unique<GameObjManager>();
	pGameObjects->Init();

	// フィールド初期化
	pMeshField = std::make_unique<MeshField>();
	pMeshField->Init(16, 14, 80.0f, 80.0f);

	// 背景初期化
	pBG = std::make_unique<BG>();
	pBG->Init();

	// 爆発初期化
	InitExplosion();

	// エフェクト初期化
	InitEffect();

	InitDwarfEffect();


	// 煙初期化
	InitSmoke();

	// タイマー初期化
	InitTimer();

	CSound::Play(BGM_GAME);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Game::Uninit() {
	// タイマー終了処理
	UninitTimer();

	// 煙終了処理
	UninitSmoke();

	// エフェクト終了処理
	UninitEffect();
	UninitDwarfEffect();

	// 爆発終了処理
	UninitExplosion();

	// 背景終了処理
	pBG->Uninit();
	pBG.reset();

	// フィールド終了処理
	pMeshField->Uninit();
	pMeshField.reset();

	// ゲームオブジェクト管理クラス終了
	pGameObjects->Uninit();
	pGameObjects.reset();

	// 丸影終了処理
	UninitShadow();

	CSound::Stop(BGM_GAME);

}

//====================================================================================
//
//				更新
//
//====================================================================================
void Game::Update() {
	// ゲームオブジェクト管理クラス更新
	pGameObjects->Update();

	// 背景更新
	pBG->Update();

	// フィールド更新
	pMeshField->Update();

	// 丸影更新
	UpdateShadow();

	// 爆発更新
	UpdateExplosion();

	// エフェクト更新
	UpdateEffect();
	UpdateDwarfEffect();

	// 煙更新
	UpdateSmoke();

	// タイマー更新
	UpdateTimer();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Game::Draw() {
	// 背景描画
	pBG->Draw();

	// フィールド描画
	pMeshField->Draw();

	// ゲームオブジェクト管理クラス描画
	pGameObjects->Draw();

	// 丸影描画
	DrawShadow();

	// 煙描画
	DrawSmoke();

	// エフェクト描画
	DrawEffect();
	DrawDwarfEffect();

	// 爆発描画
	DrawExplosion();

	// タイマー描画
	DrawTimer();
}
