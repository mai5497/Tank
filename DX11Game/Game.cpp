//************************************************************************************
// 
// �Q�[���Ǘ�[Game.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Game.h"
#include "Sound.h"
#include "shadow.h"
#include "effect.h"
#include "DwarfEffect.h"
#include "smoke.h"
#include "timer.h"
#include "input.h"

#include "BG.h"
#include "explosion.h"
#include "Meshfield.h"
#include "Fade.h"
#include "GameObjManager.h"

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Game::Game() {
	//Init();
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Game::~Game() {
	//Uninit();
}

//====================================================================================
//
//				������
//
//====================================================================================
void Game::Init() {
	// �ۉe������
	InitShadow();

	// �Q�[���I�u�W�F�N�g�Ǘ��N���X������
	pGameObjects = std::make_unique<GameObjManager>();
	pGameObjects->Init();

	// �t�B�[���h������
	pMeshField = std::make_unique<MeshField>();
	pMeshField->Init(16, 14, 80.0f, 80.0f);

	// �w�i������
	pBG = std::make_unique<BG>();
	pBG->Init();

	// ����������
	InitExplosion();

	// �G�t�F�N�g������
	InitEffect();

	InitDwarfEffect();


	// ��������
	InitSmoke();

	// �^�C�}�[������
	InitTimer();

	CSound::Play(BGM_GAME);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Game::Uninit() {
	// �^�C�}�[�I������
	UninitTimer();

	// ���I������
	UninitSmoke();

	// �G�t�F�N�g�I������
	UninitEffect();
	UninitDwarfEffect();

	// �����I������
	UninitExplosion();

	// �w�i�I������
	pBG->Uninit();
	pBG.reset();

	// �t�B�[���h�I������
	pMeshField->Uninit();
	pMeshField.reset();

	// �Q�[���I�u�W�F�N�g�Ǘ��N���X�I��
	pGameObjects->Uninit();
	pGameObjects.reset();

	// �ۉe�I������
	UninitShadow();

	CSound::Stop(BGM_GAME);

}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Game::Update() {
	// �Q�[���I�u�W�F�N�g�Ǘ��N���X�X�V
	pGameObjects->Update();

	// �w�i�X�V
	pBG->Update();

	// �t�B�[���h�X�V
	pMeshField->Update();

	// �ۉe�X�V
	UpdateShadow();

	// �����X�V
	UpdateExplosion();

	// �G�t�F�N�g�X�V
	UpdateEffect();
	UpdateDwarfEffect();

	// ���X�V
	UpdateSmoke();

	// �^�C�}�[�X�V
	UpdateTimer();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Game::Draw() {
	// �w�i�`��
	pBG->Draw();

	// �t�B�[���h�`��
	pMeshField->Draw();

	// �Q�[���I�u�W�F�N�g�Ǘ��N���X�`��
	pGameObjects->Draw();

	// �ۉe�`��
	DrawShadow();

	// ���`��
	DrawSmoke();

	// �G�t�F�N�g�`��
	DrawEffect();
	DrawDwarfEffect();

	// �����`��
	DrawExplosion();

	// �^�C�}�[�`��
	DrawTimer();
}
