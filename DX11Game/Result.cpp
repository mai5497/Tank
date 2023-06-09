//************************************************************************************
// 
// リザルト[Result.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "Result.h"
#include "Texture.h"
#include "input.h"
#include "Sound.h"
#include "number.h"
#include "Fade.h"
#include "BG.h"
#include "WallObject.h"


//-------------------- 定数定義 --------------------
#define TEX_FRAME			(L"data/texture/UI_Result.png")
#define TEX_ALLCLEAR		(L"data/texture/UI_AllClear.png")
#define TEX_STAGECHAR		(L"data/texture/UI_StageChar.png")

#define FRAME_POS_X			(0.0f)
#define FRAME_POS_Y			(20.0f)
#define FRAME_WIDTH			(620.0f)
#define FRAME_HEIGHT		(365.0f)

#define AC_WIDTH			(480.0f/2)
#define AC_HEIGHT			(75.0f/2)
#define AC_POS_X			(0.0f)
#define AC_POS_Y			(0.0f - AC_HEIGHT)

#define STAGECHAR_WIDTH		(700.0f/2)
#define STAGECHAR_HEIGHT	(190.0f/2)
#define STAGECHAR_POS_X		(0.0f - STAGECHAR_WIDTH/4)
#define STAGECHAR_POS_Y		(0.0f + STAGECHAR_HEIGHT/2)

#define STAGENUM_POS_X		(0.0f + NUMBER_SIZE_X+NUMBER_SIZE_X/2)
#define STAGENUM_POS_Y		(0.0f + STAGECHAR_HEIGHT)


std::unique_ptr<Texture> Result::pFrame;
std::unique_ptr<Texture> Result::pAllClear;
std::unique_ptr<Texture> Result::pStageChar;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Result::Result(int _stageNum) {
	stageNum = _stageNum;
}

//====================================================================================
//
//				デストラクタ
//
//====================================================================================
Result::~Result() {
}

//====================================================================================
//
//				初期化
//
//====================================================================================
void Result::Init() {
	ID3D11Device* pDevice = GetDevice();

	//----- テクスチャ読み込み -----
	pFrame = std::make_unique<Texture>();
	pFrame->SetTexture(pDevice, TEX_FRAME);

	pAllClear = std::make_unique<Texture>();
	pAllClear->SetTexture(pDevice, TEX_ALLCLEAR);

	pStageChar = std::make_unique<Texture>();
	pStageChar->SetTexture(pDevice, TEX_STAGECHAR);


	// BGM再生
	CSound::Play(BGM_RESULT);
}

//====================================================================================
//
//				終了
//
//====================================================================================
void Result::Uninit() {

	// BGM再生停止
	CSound::Stop(BGM_RESULT);

	// テクスチャ開放
	pStageChar->ReleaseTexture();
	pStageChar.reset();

	pAllClear->ReleaseTexture();
	pAllClear.reset();

	pFrame->ReleaseTexture();
	pFrame.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void Result::Update() {
	//クリックまたは[Enter]押下
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		// SE再生
		CSound::Play(SE_DECIDE);
		//セレクト画面へ
		Fade::StartFadeOut(SCENE_TITLE);
		return;
	}
}

//====================================================================================
//
//				　描画
//
//====================================================================================
void Result::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	BG::Draw(SCENE_RESULT);
	
	// Zバッファ無効(Zチェック無&Z更新無)
	SetBlendState(BS_ALPHABLEND);
	SetZBuffer(false);

	// 枠
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonSize(FRAME_WIDTH, FRAME_HEIGHT);
	SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
	SetPolygonUV(1.0f, 1.0f);
	SetPolygonTexture(pFrame->GetTexture());
	DrawPolygon(pDC);


	if (stageNum >= MAX_STAGE) {	// 全クリ
		// 「ステージ」の文字
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(STAGECHAR_WIDTH, STAGECHAR_HEIGHT);
		SetPolygonPos(STAGECHAR_POS_X, STAGECHAR_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pStageChar->GetTexture());
		DrawPolygon(pDC);

		// ステージ数
		DrawNumber(XMFLOAT2(STAGENUM_POS_X, STAGENUM_POS_Y), (unsigned)stageNum, 2);

		// AllClear
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(AC_WIDTH, AC_HEIGHT);
		SetPolygonPos(AC_POS_X, AC_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pAllClear->GetTexture());
		DrawPolygon(pDC);


	} else {
		// 「ステージ」の文字
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(STAGECHAR_WIDTH, STAGECHAR_HEIGHT);
		SetPolygonPos(STAGECHAR_POS_X, STAGECHAR_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pStageChar->GetTexture());
		DrawPolygon(pDC);

		// ステージ数
		DrawNumber(XMFLOAT2(STAGENUM_POS_X, STAGENUM_POS_Y), (unsigned)stageNum+1, 2);
	}

	// 元に戻す
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

	SetZBuffer(true);
	SetBlendState(BS_NONE);
}