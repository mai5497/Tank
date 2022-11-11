//************************************************************************************
// 
// �t�F�[�h[fade.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Fade.h"
#include "polygon.h"
//#include "Sound.h"

//-------------------- �ÓI�����o --------------------
//Scene::eSCENE Fade::eNowScene = Scene::SCENE_NONE;


//-------------------- �萔��` --------------------
#define FADE_RATE	0.02f		// �t�F�[�h�C���t�F�[�h�A�E�g�̑��x

//-------------------- �O���[�o���ϐ���` --------------------
static float g_fRed = 1.0f;		// �t�F�[�h�A�E�g�F
static float g_fGreen = 1.0f;
static float g_fBlue = 1.0f;
static float g_fAlpha = 1.0f;	// �����x
static EFade g_eFade = FADE_IN;
static eSCENE g_eNext = SCENE_TITLE;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Fade::Fade(){
	//eNowScene = Scene::SCENE_NONE;
	Init();
}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Fade::~Fade() {
	Uninit();
}



//====================================================================================
//
//				������
//
//====================================================================================
void Fade::Init() {
	g_eFade = FADE_IN;
	g_fAlpha = 1.0f;
	g_eNext = SCENE_TITLE;
}


//====================================================================================
//
//				�I��
//
//====================================================================================
void Fade::Uninit() {
	g_eFade = FADE_NONE;
	g_fAlpha = 0.0f;
}


//====================================================================================
//
//				�X�V
//
//====================================================================================
void Fade::Update() {
	switch (g_eFade) {
	case FADE_NONE:
		break;
	case FADE_OUT:
		g_fAlpha += FADE_RATE;		// �s�����x�𑝂�
		if (g_fAlpha >= 1.0f) {
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
			SceneManager::SetScene(g_eNext);
		}
		//CSound::SetVolume(1.0f - g_fAlpha);
		break;
	case FADE_IN:
		g_fAlpha -= FADE_RATE;		// �����x�𑝂�
		if (g_fAlpha <= 0.0f) {
			// �t�F�[�h�C�����I������
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		// �{�����[�����t�F�[�h�C��
		//CSound::SetVolume(1.0f - g_fAlpha);
		break;
	default:
		break;
	}
}


//====================================================================================
//
//				�`��
//
//====================================================================================
void Fade::Draw() {
	SetBlendState(BS_ADDITIVE);		// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)

	// ��ʑS�̂ɔ������̋�`��`��
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(nullptr);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);

	SetZWrite(true);
	SetBlendState(BS_NONE);
}



//====================================================================================
//
//				�t�F�[�h�A�E�g�J�n
//
//====================================================================================
void Fade::StartFadeOut(eSCENE eScene) {
	if (g_eFade != FADE_OUT) {
		//eNowScene = eScene;
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eScene;
	}
}


//====================================================================================
//
//				��Ԏ擾
//
//====================================================================================
EFade Fade::GetFade() {
	return g_eFade;
}


//====================================================================================
//
//				�t�F�[�h�F�ݒ�
//
//====================================================================================
void Fade::SetFadeColor(float fR, float fG, float fB) {
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}
