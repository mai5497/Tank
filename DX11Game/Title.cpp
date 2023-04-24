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
#include "Sound.h"
#include "Fade.h"
#include "TitleLogo.h"
#include "PressSpace.h"
#include "BG.h"
#include "meshfield.h"
#include "Particle.h"

//-------------------- �萔��` --------------------
#define TITLE_POS_X			(0.0f)
#define TITLE_POS_Y			(0.0f)
#define TITLE_WIDTH			(SCREEN_WIDTH)
#define TITLE_HEIGHT		(SCREEN_HEIGHT)
#define MOVIETIMER			(600)
#define FADETIMER			(120)

//-------------------- �O���[�o���ϐ���` --------------------
static LPCWSTR g_pszTexFName = {
	L"data/texture/title.png"
};

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Title::Title() {
	//Init();
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Title::~Title() {
	//Uninit();
}


//====================================================================================
//
//				������
//
//====================================================================================
void Title::Init() {
	ID3D11Device *pDevice = GetDevice();

	fadeTimer = FADETIMER;
	isFade = false;
	// ���S������
	pLogo = std::make_unique<TitleLogo>();
	pLogo->Init();

	// �v���X�X�y�[�X������
	pStart = std::make_unique<PressSpace>();
	pStart->Init();

	// �p�[�e�B�N��������
	pParticle = std::make_unique<Particle>();
	pParticle->Init();

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

	// �p�[�e�B�N���I������
	pParticle->Uninit();
	pParticle.reset();

	// �v���X�X�y�[�X�I��
	pStart->Uninit();
	pStart.reset();

	// ���S�̏I��
	pLogo->Uninit();
	pLogo.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Title::Update() {
	//----- �V�[���J�� -----
	if (!isFade) {	// ���͂���x�����󂯕t����
		// �N���b�N�܂���[Space]����
		if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
			CSound::Play(SE_DECIDE);
			isFade = true;
		}
	}

	// �V�[���J�ڂ���t�F�[�h���s���܂ŏ����҂�
	if (isFade) {
		fadeTimer--;
		if (fadeTimer < 1) {
			//���[�h�I����ʂ�
			Fade::StartFadeOut(SCENE_MODESELECT);
			return;
		}
	}

	//----- UI�̍X�V -----
	resetTimer--;
	if (resetTimer > 0) {
		// ���S�̍X�V
		pLogo->Update();

		// �v���X�X�y�[�X�̍X�V
		pStart->Update();

		// �p�[�e�B�N��
		pParticle->Update();

	} else {	// 0�ɂȂ����烊�Z�b�g���Ă�����x������
		pLogo->Reset();

		resetTimer = MOVIETIMER;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Title::Draw() {

	// �w�i�̕`��
	BG::Draw(SCENE_TITLE);

	// ���S�̕`��
	pLogo->Draw();

	// �v���X�X�y�[�X�̕`��
	pStart->Draw();

	// �p�[�e�B�N���̕`��
	pParticle->Draw();
}