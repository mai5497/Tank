//************************************************************************************
// 
// �^�C�g��[Title.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "fade.h"
#include "Sound.h"

//-------------------- �萔��` --------------------
#define TITLE_POS_X			0.0f
#define TITLE_POS_Y			0.0f
#define TITLE_WIDTH			SCREEN_WIDTH
#define TITLE_HEIGHT		SCREEN_HEIGHT

//-------------------- �O���[�o���ϐ���` --------------------
static LPCWSTR g_pszTexFName = {
	L"data/texture/title.png"
};

static ID3D11ShaderResourceView *g_pTexture;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Title::Title() {
	Init();
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Title::~Title() {
	Uninit();
}


//====================================================================================
//
//				������
//
//====================================================================================
void Title::Init() {
	ID3D11Device *pDevice = GetDevice();
	CreateTextureFromFile(pDevice, g_pszTexFName, &g_pTexture);
	nowScene = Scene::SCENE_TITLE;

	// BGM�Đ�
	CSound::Play(BGM_TITLE);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Title::Uninit() {

	// BGM�Đ���~
	CSound::Stop(BGM_TITLE);
	// �w�i�e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Title::Update() {
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
		CSound::Play(SE_DECIDE);
		//���[�h�I����ʂ�
		//SetScene(SCENE_GAME);
		Fade::StartFadeOut(Scene::SCENE_GAME);
		return;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Title::Draw() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);

	DrawPolygon(pDC);
}