#include "GameSummary.h"

#include "Texture.h"
#include "input.h"
#include "fade.h"

#include "bg.h"


#define PATH_UI_TEXTURE	L"data/texture/uiSousa.png"

#define UI_WIDTH		(615)
#define UI_HEIGHT		(401)

#define SCENE_CHANGE_TIME	(180)


// �ÓI�����o�ϐ�������
std::unique_ptr<Texture> GameSummary::pBG;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
GameSummary::GameSummary() {

}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
GameSummary::~GameSummary() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void GameSummary::Init() {
	ID3D11Device* pDevice = GetDevice();

	//pBG = std::make_unique<Texture>();
	//pBG->SetTexture(pDevice, PATH_UI_TEXTURE);

	sceneChangeTimer = SCENE_CHANGE_TIME;
}


//====================================================================================
//
//				�I��
//
//====================================================================================
void GameSummary::Uninit() {
	//pBG->ReleaseTexture();
	//pBG.reset();
}


//====================================================================================
//
//				�X�V
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
//				�`��
//
//====================================================================================
void GameSummary::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	BG::Draw(SCENE_STAGESUMMARY);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	//SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	//SetPolygonFrameSize(1.0f, 1.0f);
	//SetPolygonTexture(pBG->GetTexture());
	//SetPolygonPos(0.0f, 0.0f);
	//SetPolygonUV(0.0f, 0.0f);
	//DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}