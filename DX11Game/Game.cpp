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
#include "bullet.h"
#include "explosion.h"
#include "Meshfield.h"
#include "Fade.h"
#include "WallObject.h"
//#include "Boss.h"
#include "player.h"
#include "EnemyManager.h"

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Game::Game() {
	Init();
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Game::~Game() {
	Uninit();
}

//====================================================================================
//
//				������
//
//====================================================================================
void Game::Init() {
	//nowScene = Scene::SCENE_GAME;

	// �ۉe������
	InitShadow();

	// ���@������
	pPlayer = std::make_unique<Player>();
	pPlayer->Init();

	// �G������
	pEnemys = std::make_shared<EnemyManager>();
	pEnemys->Init();

	//InitBoss();

	// �t�B�[���h������
	pMeshField = std::make_unique<MeshField>();
	pMeshField->Init(16, 14, 80.0f, 80.0f);

	// �w�i������
	pBG = std::make_unique<BG>();
	pBG->Init();

	// �r���{�[�h�e������
	InitBullet();

	// ����������
	InitExplosion();

	// �G�t�F�N�g������
	InitEffect();

	InitDwarfEffect();


	// ��������
	InitSmoke();

	// �Ǐ�����
	//hr = InitMeshWall();
	//if (FAILED(hr))
	//	return hr;

	// �^�C�}�[������
	InitTimer();

	// �I�u�W�F�N�g�̕Ǐ�����
	InitWallObj();

	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	CSound::Play(BGM_GAME);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Game::Uninit() {
	// �I�u�W�F�N�g�ǏI��
	UninitWallObj();

	// �^�C�}�[�I������
	UninitTimer();

	// �ǏI������
	//UninitMeshWall();

	// ���I������
	UninitSmoke();

	// �G�t�F�N�g�I������
	UninitEffect();
	UninitDwarfEffect();

	// �����I������
	UninitExplosion();

	// �r���{�[�h�e�I������
	UninitBullet();

	// �w�i�I������
	pBG->Uninit();
	pBG.reset();

	// �t�B�[���h�I������
	pMeshField->Uninit();
	pMeshField.reset();

	// ���@�I������
	pPlayer->Uninit();
	pPlayer.reset();

	// �G�@�I������
	//UninitBoss();
	pEnemys->Uninit();
	pEnemys.reset();

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
	// ���@�X�V
	pPlayer->Update();

	// �G�X�V
	pEnemys->Update();
	//if (pEnemys->GetEnemyKillSum() == MAX_ENEMY) {
		//bool isBossAlive = UpdateBoss();
		//if (!isBossAlive) {
			//Fade::StartFadeOut(SCENE_RESULT);
		//}
	//}


	// �w�i�X�V
	pBG->Update();

	// �t�B�[���h�X�V
	pMeshField->Update();

	// �ۉe�X�V
	UpdateShadow();

	// �r���{�[�h�e�X�V
	UpdateBullet();

	// �����X�V
	UpdateExplosion();

	// �G�t�F�N�g�X�V
	UpdateEffect();
	UpdateDwarfEffect();

	// ���X�V
	UpdateSmoke();

	// �ǍX�V
	//UpdateMeshWall();

	// �^�C�}�[�X�V
	UpdateTimer();

	// �I�u�W�F�N�g�̕ǍX�V
	UpdateWallObj();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Game::Draw() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	pBG->Draw();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	// �t�B�[���h�`��
	pMeshField->Draw();

	// �Ǖ`�� (�s��������)
	//DrawMeshWall(DRAWPART_OPAQUE);

	// ���@�`��
	pPlayer->Draw();

	// �G�@�`��
	pEnemys->Draw();
	if (pEnemys->GetEnemyKillSum() == MAX_ENEMY) {
		//DrawBoss();
	}

	// �I�u�W�F�N�g�̕Ǖ`��
	DrawWallObj();

	// �ۉe�`��
	DrawShadow();

	// �r���{�[�h�e�`��
	DrawBullet();

	// ���`��
	DrawSmoke();

	// �G�t�F�N�g�`��
	DrawEffect();
	DrawDwarfEffect();

	// �����`��
	DrawExplosion();

	// �Ǖ`�� (����������)
	//DrawMeshWall(DRAWPART_TRANSLUCENT);
		// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �^�C�}�[�`��
	DrawTimer();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

}
