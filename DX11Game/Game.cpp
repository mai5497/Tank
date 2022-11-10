//************************************************************************************
// 
// �Q�[���Ǘ�[Game.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Game.h"
#include "shadow.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "smoke.h"
//#include "meshwall.h"
#include "enemy.h"
#include "meshfield.h"
#include "timer.h"
#include "fade.h"
#include "Sound.h"
#include "DwarfEffect.h"
#include "WallObject.h"
//#include "Boss.h"

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

	// �G������
	InitEnemy();

	//InitBoss();

	// �t�B�[���h������
	InitMeshField(16, 14, 80.0f, 80.0f);

	// �w�i������
	InitBG();

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
	UninitBG();

	// �t�B�[���h�I������
	UninitMeshField();

	// ���@�I������
	pPlayer.reset();

	// �G�@�I������
	//UninitBoss();
	UninitEnemy();

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
	UpdateEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
		//bool isBossAlive = UpdateBoss();
		//if (!isBossAlive) {
			Fade::StartFadeOut(Scene::SCENE_RESULT);
		//}
	}


	// �w�i�X�V
	UpdateBG();

	// �t�B�[���h�X�V
	UpdateMeshField();

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
	DrawBG();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	// �t�B�[���h�`��
	DrawMeshField();

	// �Ǖ`�� (�s��������)
	//DrawMeshWall(DRAWPART_OPAQUE);

	// ���@�`��
	pPlayer->Draw();

	// �G�@�`��
	DrawEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
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
