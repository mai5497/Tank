//************************************************************************************
// 
// フェード[fade.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Fade.h"
#include "polygon.h"
//#include "Sound.h"

//-------------------- 静的メンバ --------------------
//Scene::eSCENE Fade::eNowScene = Scene::SCENE_NONE;


//-------------------- 定数定義 --------------------
#define FADE_RATE	0.02f		// フェードインフェードアウトの速度

//-------------------- グローバル変数定義 --------------------
static float g_fRed = 1.0f;		// フェードアウト色
static float g_fGreen = 1.0f;
static float g_fBlue = 1.0f;
static float g_fAlpha = 1.0f;	// 透明度
static EFade g_eFade = FADE_IN;
static Scene::eSCENE g_eNext = Scene::SCENE_TITLE;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Fade::Fade(){
	//eNowScene = Scene::SCENE_NONE;
	Init();
}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Fade::~Fade() {
	Uninit();
}



//====================================================================================
//
//				初期化
//
//====================================================================================
void Fade::Init() {
	g_eFade = FADE_IN;
	g_fAlpha = 1.0f;
	g_eNext = Scene::SCENE_TITLE;
}


//====================================================================================
//
//				終了
//
//====================================================================================
void Fade::Uninit() {
	g_eFade = FADE_NONE;
	g_fAlpha = 0.0f;
}


//====================================================================================
//
//				更新
//
//====================================================================================
void Fade::Update() {
	switch (g_eFade) {
	case FADE_NONE:
		break;
	case FADE_OUT:
		g_fAlpha += FADE_RATE;		// 不透明度を増す
		if (g_fAlpha >= 1.0f) {
			// フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
			Scene::SetScene(g_eNext);
		}
		//CSound::SetVolume(1.0f - g_fAlpha);
		break;
	case FADE_IN:
		g_fAlpha -= FADE_RATE;		// 透明度を増す
		if (g_fAlpha <= 0.0f) {
			// フェードインを終了する
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		// ボリュームもフェードイン
		//CSound::SetVolume(1.0f - g_fAlpha);
		break;
	default:
		break;
	}
}


//====================================================================================
//
//				描画
//
//====================================================================================
void Fade::Draw() {
	SetBlendState(BS_ADDITIVE);		// 加算合成
	SetZWrite(false);	// 半透明描画はZバッファを更新しない(Zチェックは行う)

	// 画面全体に半透明の矩形を描画
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(nullptr);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);

	SetZWrite(true);
	SetBlendState(BS_NONE);
}



//====================================================================================
//
//				フェードアウト開始
//
//====================================================================================
void Fade::StartFadeOut(Scene::eSCENE eScene) {
	if (g_eFade != FADE_OUT) {
		//eNowScene = eScene;
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eScene;
	}
}


//====================================================================================
//
//				状態取得
//
//====================================================================================
EFade Fade::GetFade() {
	return g_eFade;
}


//====================================================================================
//
//				フェード色設定
//
//====================================================================================
void Fade::SetFadeColor(float fR, float fG, float fB) {
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}
