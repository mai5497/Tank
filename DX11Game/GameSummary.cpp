#include "GameSummary.h"

#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "number.h"

#include "BG.h"
#include "WallObject.h"





#define TEX_STAGECHAR		(L"data/texture/UI_StageChar.png")
#define TEX_KAKERUCHAR		(L"data/texture/UI_Kakeru.png")
#define TEX_ENEMYICON		(L"data/texture/UI_EnemyIcon.png")

#define UI_BASEPOS_X		(50.0f)
#define UI_BASEPOS_Y		(50.0f)

#define STAGECHAR_WIDTH		(700.0f/2)
#define STAGECHAR_HEIGHT	(190.0f/2)
#define STAGECHAR_POS_X		(UI_BASEPOS_X - STAGECHAR_WIDTH/2)
#define STAGECHAR_POS_Y		(UI_BASEPOS_Y + STAGECHAR_HEIGHT/2)

#define STAGENUM_POS_X		(UI_BASEPOS_X + NUMBER_SIZE_X)
#define STAGENUM_POS_Y		(UI_BASEPOS_Y + STAGECHAR_HEIGHT)

#define KAKERU_WIDTH		(90.0f)
#define KAKERU_HEIGHT		(90.0f)
#define KAKERU_POS_X		(UI_BASEPOS_X - KAKERU_WIDTH/2)	
#define KAKERU_POS_Y		(UI_BASEPOS_Y - NUMBER_SIZE_Y-NUMBER_SIZE_Y/2)

#define ENEMYCNTICON_WIDTH		(100.0f)
#define ENEMYCNTICON_HEIGHT		(185.0f)
#define ENEMYCNTICON_POS_X		(KAKERU_POS_X - ENEMYCNTICON_WIDTH)
#define ENEMYCNTICON_POS_Y		(UI_BASEPOS_Y - NUMBER_SIZE_Y-NUMBER_SIZE_Y/2)



#define ENEMYCNT_POS_X			(UI_BASEPOS_X + NUMBER_SIZE_X)
#define ENEMYCNT_POS_Y			(UI_BASEPOS_Y - NUMBER_SIZE_Y)

#define SCENE_CHANGE_TIME	(180)


// 静的メンバ変数初期化
std::unique_ptr<Texture> GameSummary::pStageChar;
std::unique_ptr<Texture> GameSummary::pKakeruChar;
std::unique_ptr<Texture> GameSummary::pEnemyIcon;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
GameSummary::GameSummary(int _stageNum) {
	stageNum = _stageNum;
}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
GameSummary::~GameSummary() {

}


//====================================================================================
//
//				初期化
//
//====================================================================================
void GameSummary::Init() {
	ID3D11Device* pDevice = GetDevice();

	pStageChar = std::make_unique<Texture>();
	pStageChar->SetTexture(pDevice, TEX_STAGECHAR);

	pKakeruChar = std::make_unique<Texture>();
	pKakeruChar->SetTexture(pDevice, TEX_KAKERUCHAR);

	pEnemyIcon = std::make_unique<Texture>();
	pEnemyIcon->SetTexture(pDevice, TEX_ENEMYICON);

	sceneChangeTimer = SCENE_CHANGE_TIME;

	//----- 敵の総数カウント -----
	enemyCnt = 0;
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (WallObj::wallMap[stageNum][j][i] == 2) {
				enemyCnt++;
			}
		}
	}

}


//====================================================================================
//
//				終了
//
//====================================================================================
void GameSummary::Uninit() {
	pEnemyIcon->ReleaseTexture();
	pEnemyIcon.reset();

	pKakeruChar->ReleaseTexture();
	pKakeruChar.reset();

	pStageChar->ReleaseTexture();
	pStageChar.reset();

}


//====================================================================================
//
//				更新
//
//====================================================================================
void GameSummary::Update() {
	sceneChangeTimer--;
	if (sceneChangeTimer < 0) {
		Fade::StartFadeOut(SCENE_GAME);
		sceneChangeTimer = SCENE_CHANGE_TIME;
	}
}


//====================================================================================
//
//				描画
//
//====================================================================================
void GameSummary::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	BG::Draw(SCENE_STAGESUMMARY);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

	SetPolygonSize(STAGECHAR_WIDTH, STAGECHAR_HEIGHT);
	SetPolygonTexture(pStageChar->GetTexture());
	SetPolygonPos(STAGECHAR_POS_X, STAGECHAR_POS_Y);
	DrawPolygon(pDC);

	// ステージ数
	DrawNumber(XMFLOAT2(STAGENUM_POS_X, STAGENUM_POS_Y), (unsigned)stageNum + 1, 2);


	SetPolygonSize(ENEMYCNTICON_WIDTH, ENEMYCNTICON_HEIGHT);
	SetPolygonTexture(pEnemyIcon->GetTexture());
	SetPolygonPos(ENEMYCNTICON_POS_X, ENEMYCNTICON_POS_Y);
	DrawPolygon(pDC);

	SetPolygonSize(KAKERU_WIDTH, KAKERU_HEIGHT);
	SetPolygonTexture(pKakeruChar->GetTexture());
	SetPolygonPos(KAKERU_POS_X, KAKERU_POS_Y);
	DrawPolygon(pDC);

	//----- 敵総数 -----
	DrawNumber(XMFLOAT2(ENEMYCNT_POS_X, ENEMYCNT_POS_Y), (unsigned)enemyCnt, 2);


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}