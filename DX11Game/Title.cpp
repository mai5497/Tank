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

static ID3D11ShaderResourceView *g_pTexture;

//====================================================================================
//
//				�R���X�g���N�^
//
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
	CreateTextureFromFile(pDevice, g_pszTexFName, &g_pTexture);
	//nowScene = Scene::SCENE_TITLE;

	timer = MOVIETIMER;
	fadeTimer = FADETIMER;
	isFade = false;
	// ���S������
	pLogo = std::make_unique<TitleLogo>();
	pLogo->Init();

	// �v���X�X�y�[�X������
	pStart = std::make_unique<PressSpace>();
	pStart->Init();

	// �w�i������
	pBG = std::make_unique<BG>();
	pBG->Init();

	// �t�B�[���h������
	pMeshField = std::make_unique<MeshField>();
	pMeshField->Init(8, 7, 80.0f, 80.0f);

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
	// �w�i�e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);

	// �p�[�e�B�N���I������
	pParticle->Uninit();
	pParticle.reset();

	// �t�B�[���h�I������
	pMeshField->Uninit();
	pMeshField.reset();

	// �w�i�I��
	pBG->Uninit();
	pBG.reset();

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
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_SPACE)) {
		CSound::Play(SE_DECIDE);
		isFade = true;
	}
	if (isFade) {
		fadeTimer--;
		if (fadeTimer < 1) {
			//���[�h�I����ʂ�
			//SetScene(SCENE_GAME);
			Fade::StartFadeOut(SCENE_GAME);
			return;
		}
	}
	timer--;
	if (timer > 0) {
		// ���S�̍X�V
		pLogo->Update();

		// �v���X�X�y�[�X�̍X�V
		pStart->Update();

		// �t�B�[���h�X�V
		pMeshField->Update();

		// �p�[�e�B�N��
		pParticle->Update();

	} else {
		pLogo->Reset();

		timer = MOVIETIMER;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Title::Draw() {
	//// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	//SetZBuffer(false);

	//ID3D11DeviceContext *pDC = GetDeviceContext();
	//SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	//SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	//SetPolygonTexture(g_pTexture);
	//SetPolygonUV(0.0f, 0.0f);

	//DrawPolygon(pDC);


	// �w�i�̕`��
	pBG->Draw();

	// �t�B�[���h�`��
	pMeshField->Draw();

	// ���S�̕`��
	pLogo->Draw();

	// �v���X�X�y�[�X�̕`��
	pStart->Draw();

	// �p�[�e�B�N���̕`��
	pParticle->Draw();
}