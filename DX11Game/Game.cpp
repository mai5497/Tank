//************************************************************************************
// 
// ゲーム管理[Game.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Game.h"
#include "shadow.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "smoke.h"
//#include "meshwall.h"
#include "enemy.h"
#include "meshfield.h"
#include "timer.h"
#include "fade.h"
#include "Sound.h"
#include "DwarfEffect.h"
#include "WallObject.h"
//#include "Boss.h"

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Game::Game() {
	Init();
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Game::~Game() {
	Uninit();
}

//====================================================================================
//
//				初期化
//
//====================================================================================
void Game::Init() {
	//nowScene = Scene::SCENE_GAME;

	// 丸影初期化
	InitShadow();

	// 自機初期化
	pPlayer = std::make_unique<Player>();

	// 敵初期化
	InitEnemy();

	//InitBoss();

	// フィールド初期化
	InitMeshField(16, 14, 80.0f, 80.0f);

	// 背景初期化
	InitBG();

	// ビルボード弾初期化
	InitBullet();

	// 爆発初期化
	InitExplosion();

	// エフェクト初期化
	InitEffect();

	InitDwarfEffect();


	// 煙初期化
	InitSmoke();

	// 壁初期化
	//hr = InitMeshWall();
	//if (FAILED(hr))
	//	return hr;

	// タイマー初期化
	InitTimer();

	// オブジェクトの壁初期化
	InitWallObj();

	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	CSound::Play(BGM_GAME);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Game::Uninit() {
	// オブジェクト壁終了
	UninitWallObj();

	// タイマー終了処理
	UninitTimer();

	// 壁終了処理
	//UninitMeshWall();

	// 煙終了処理
	UninitSmoke();

	// エフェクト終了処理
	UninitEffect();
	UninitDwarfEffect();

	// 爆発終了処理
	UninitExplosion();

	// ビルボード弾終了処理
	UninitBullet();

	// 背景終了処理
	UninitBG();

	// フィールド終了処理
	UninitMeshField();

	// 自機終了処理
	pPlayer.reset();

	// 敵機終了処理
	//UninitBoss();
	UninitEnemy();

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
	// 自機更新
	pPlayer->Update();

	// 敵更新
	UpdateEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
		//bool isBossAlive = UpdateBoss();
		//if (!isBossAlive) {
			Fade::StartFadeOut(Scene::SCENE_RESULT);
		//}
	}


	// 背景更新
	UpdateBG();

	// フィールド更新
	UpdateMeshField();

	// 丸影更新
	UpdateShadow();

	// ビルボード弾更新
	UpdateBullet();

	// 爆発更新
	UpdateExplosion();

	// エフェクト更新
	UpdateEffect();
	UpdateDwarfEffect();

	// 煙更新
	UpdateSmoke();

	// 壁更新
	//UpdateMeshWall();

	// タイマー更新
	UpdateTimer();

	// オブジェクトの壁更新
	UpdateWallObj();
}

//====================================================================================
//
//				描画
//
//====================================================================================
void Game::Draw() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	DrawBG();

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	// フィールド描画
	DrawMeshField();

	// 壁描画 (不透明部分)
	//DrawMeshWall(DRAWPART_OPAQUE);

	// 自機描画
	pPlayer->Draw();

	// 敵機描画
	DrawEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
		//DrawBoss();
	}

	// オブジェクトの壁描画
	DrawWallObj();

	// 丸影描画
	DrawShadow();

	// ビルボード弾描画
	DrawBullet();

	// 煙描画
	DrawSmoke();

	// エフェクト描画
	DrawEffect();
	DrawDwarfEffect();

	// 爆発描画
	DrawExplosion();

	// 壁描画 (半透明部分)
	//DrawMeshWall(DRAWPART_TRANSLUCENT);
		// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// タイマー描画
	DrawTimer();

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

}
