//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

//フェードイン・フェードアウト
#pragma once

#include "SceneManager.h"


enum eFade {
	FADE_NONE = 0,		// 何もしていない
	FADE_IN,			// フェードイン処理中
	FADE_OUT,			// フェードアウト処理中

	MAX_FADE			// フェード最大値
};


class Fade {
public:
	Fade();
	~Fade();

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	static void StartFadeOut(eSCENE eScene);						// フェードアウト開始

	eFade GetFade();									// 状態取得
	void SetFadeColor(float fR, float fG, float fB);	// フェードアウト色設定

private:
	float red;		// フェードアウト色
	float green;
	float blue;
	static eFade stateFade;
	eFade oldStateFade;
	static eSCENE nextScene;

	//static Scene::eSCENE eNowScene;
};