#include "Game.h"
#include "player.h"
#include "shadow.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "smoke.h"
#include "meshwall.h"
#include "enemy.h"
#include "meshfield.h"
#include "timer.h"
#include "fade.h"
#include "Sound.h"
#include "DwarfEffect.h"
#include "Boss.h"


HRESULT InitGame() {
	HRESULT hr = S_OK;

	// �ۉe������
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	// ���@������
	hr = InitPlayer();
	if (FAILED(hr))
		return hr;

	// �G������
	hr = InitEnemy();
	if (FAILED(hr)) {
		return hr;
	}
	hr = InitBoss();
	if (FAILED(hr)) {
		return hr;
	}

	// �t�B�[���h������
	hr = InitMeshField(16, 16, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;

	// �w�i������
	hr = InitBG();
	if (FAILED(hr))
		return hr;

	// �r���{�[�h�e������
	hr = InitBullet();
	if (FAILED(hr))
		return hr;

	// ����������
	hr = InitExplosion();
	if (FAILED(hr))
		return hr;

	// �G�t�F�N�g������
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	hr = InitDwarfEffect();
	if (FAILED(hr))
		return hr;


	// ��������
	hr = InitSmoke();
	if (FAILED(hr))
		return hr;

	// �Ǐ�����
	hr = InitMeshWall();
	if (FAILED(hr))
		return hr;

	// �^�C�}�[������
	hr = InitTimer();
	if (FAILED(hr))
		return hr;

	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	CSound::Play(BGM_GAME);

	return hr;

}
void UninitGame() {
	// �^�C�}�[�I������
	UninitTimer();

	// �ǏI������
	UninitMeshWall();

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
	UninitPlayer();

	// �G�@�I������
	UninitBoss();
	UninitEnemy();

	// �ۉe�I������
	UninitShadow();

	CSound::Stop(BGM_GAME);

}

void UpdateGame() {
	// ���@�X�V
	UpdatePlayer();

	// �G�X�V
	UpdateEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
		bool isBossAlive = UpdateBoss();
		if (!isBossAlive) {
			StartFadeOut(SCENE_RESULT);
		}
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
	UpdateMeshWall();

	// �^�C�}�[�X�V
	UpdateTimer();

}
void DrawGame() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	DrawBG();

	// �^�C�}�[�`��
	DrawTimer();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	// �t�B�[���h�`��
	DrawMeshField();

	// �Ǖ`�� (�s��������)
	DrawMeshWall(DRAWPART_OPAQUE);

	// ���@�`��
	DrawPlayer();

	// �G�@�`��
	DrawEnemy();
	if (GetEnemyKillSum() == MAX_ENEMY) {
		DrawBoss();
	}

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
	DrawMeshWall(DRAWPART_TRANSLUCENT);

}
