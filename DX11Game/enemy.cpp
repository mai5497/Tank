//************************************************************************************
// 
// �G[enemy.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "enemy.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "explosion.h"
#include "DwarfEffect.h"
#include "Sound.h"
#include "bullet.h"
#include "collision.h"
#include "Astar.h"

//-------------------- �萔��` --------------------
#define MODEL_ENEMY			"data/model/kobitored.fbx"

#define	VALUE_MOVE_ENEMY	(0.40f)		// �ړ����x
#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��
#define ENEMY_RADIUS		(50.0f)		// ���E�����a

#define BULLET_TIME			(300)		// �e���˂܂ł̎���
#define ROOT_TIME			(300)		// ���Ƀ��[�g��������܂ł̎���


//-------------------- �\���̒�` --------------------

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;			// ���f��

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Enemy::Enemy() {
	Init();
}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Enemy::~Enemy() {

}

//====================================================================================
//
//				������
//
//====================================================================================
void Enemy::Init() {
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
	}

	// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
	pos = XMFLOAT3(rand() % 620 - 310.0f, 0.0f, rand() % 620 - 310.0f);
	rotModel = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rotModel;
	moveVal = XMFLOAT3(
		-SinDeg(rotModel.y) * VALUE_MOVE_ENEMY,
		0.0f,
		-CosDeg(rotModel.y) * VALUE_MOVE_ENEMY);
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	// �ۉe�̐���
	shadowNum = CreateShadow(pos, 25.0f);
	use = true;
	bulletTimer = 300;

	mapIndex = XMINT2(0, 0);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Enemy::Uninit() {
	//for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ۉe�̉��
	ReleaseShadow(shadowNum);
	//}

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Enemy::Update() {
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//for (int i = 0; i < MAX_ENEMY; ++i) {
	if (!use) {
		return;
	}
	moveVal = XMFLOAT3(
		-SinDeg(rotModel.y) * VALUE_MOVE_ENEMY,
		0.0f,
		-CosDeg(rotModel.y) * VALUE_MOVE_ENEMY);

	if (moveVal.x > 0.0f) {
		ShadowMove.x += 25.0f;
	} else if (moveVal.x < 0.0f) {
		ShadowMove.x -= 25.0f;
	}

	if (moveVal.z > 0.0f) {
		ShadowMove.z += 25.0f;
	} else if (moveVal.z < 0.0f) {
		ShadowMove.z -= 25.0f;
	}


	// �ړ�
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	// �ǂɂԂ�����
	bool lr = false, fb = false;
	if (pos.x < -310.0f) {
		pos.x = -310.0f;
		lr = true;
	}
	if (pos.x > 310.0f) {
		pos.x = 310.0f;
		lr = true;
	}
	if (pos.z < -310.0f) {
		pos.z = -310.0f;
		fb = true;
	}
	if (pos.z > 310.0f) {
		pos.z = 310.0f;
		fb = true;
	}
	if (pos.y < 0.0f) {
		pos.y = 0.0f;
	}
	if (pos.y > 80.0f) {
		pos.y = 80.0f;
	}
	if (fabsf(rotModel.y - rotDest.y) < 0.0001f) {
		if (lr) {
			moveVal.x *= -1.0f;
		}
		if (fb) {
			moveVal.z *= -1.0f;
		}
		if (lr || fb) {
			rotDest.y = XMConvertToDegrees(atan2f(-moveVal.x, -moveVal.z));
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = rotDest.y - rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	rotModel.y += fDiffRotY * RATE_ROTATE_ENEMY;
	if (rotModel.y >= 180.0f) {
		rotModel.y -= 360.0f;
	}
	if (rotModel.y < -180.0f) {
		rotModel.y += 360.0f;
	}


	// �}�b�v�̗v�f�ԍ��ł������猻�݂̈ʒu���ǂ��ɂȂ�̂������߂�
	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;


	// ���[���h�}�g���b�N�X�̏�����
	_mtxWorld = XMMatrixIdentity();

	/* ���[���h���W�̕ϊ��͕K���A�g��A��]�A�ړ��̏��ōs�� */

	// �X�P�[���𔽉f
	_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
	_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);


	// ��]�𔽉f
	_mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(rotModel.x),
		XMConvertToRadians(rotModel.y),
		XMConvertToRadians(rotModel.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// �ړ��𔽉f
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&mtxWorld, _mtxWorld);

	// �ۉe�̈ړ�
	MoveShadow(shadowNum, XMFLOAT3(pos.x + ShadowMove.x, pos.y, pos.z + ShadowMove.z));

	int randomtime;
	randomtime = rand() % 3;
	bulletTimer -= randomtime;
	if (bulletTimer < 0) {
		FireBullet(
			pos,
			XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLETTYPE_ENEMY);

		bulletTimer = BULLET_TIME;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Enemy::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	if (!use) {
		return;
	}

	// �s����������`��
	g_model.Draw(pDC, mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	g_model.Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//====================================================================================
//
//				�����蔻��
//
//====================================================================================
//int CollisionEnemy(XMFLOAT3 pos, float radius, float damage) {
//	int hitNum = 0;
//	for (int i = 0; i < MAX_ENEMY; i++) {
//		if (!g_enemy[i].m_use) {
//			continue;
//		}
//		bool hit = CollisionSphere(g_GameObj[i]->pos, ENEMY_RADIUS, pos, radius);
//		if (hit) {
//			hitNum++;
//			// �����J�n
//			int nExp = -1;
//			if (damage > 0.0f) {
//				StartDwarfEffect(g_GameObj[i]->pos);
//				//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
//
//			} else {
//				nExp = StartExplosion(g_GameObj[i]->pos, XMFLOAT2(20.0f, 20.0f));
//			}
//			SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
//
//			g_enemyKillSum++;
//			g_enemy[i].m_use = false;
//			ReleaseShadow(g_enemy[i].m_nShadow);
//			CSound::Play(SE_KILL);
//		}
//	}
//	return hitNum;
//}
//
//void CollisionEnemy(GameObject collision) {
//	GameObject* _GameObject;
//
//	for (int i = 0; i < MAX_ENEMY; i++) {
//		bool isHit = CollisionSphere(*g_GameObj[i], collision);
//
//		if (isHit) {
//			_GameObject = Push(g_GameObj[i]->pos, XMFLOAT3(10.0f, 10.0f, 10.0f), g_GameObj[i]->moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));
//
//			if (_GameObject == nullptr) {
//				//�@�����o���Ɏ��s���Ă���
//				return;
//			}
//
//			g_GameObj[i]->pos = _GameObject->pos;
//			g_GameObj[i]->moveVal = _GameObject->moveVal;
//		}
//	}
//}



//====================================================================================
//
//				���Ɉړ�����ׂ��ꏊ���i�[����index��ۑ�����
//
//====================================================================================
void Enemy::SetRootIndex(XMINT2 index) {
	mapIndex = index;
}

//====================================================================================
//
//				A*�̊J�n�n�_�������
//
//====================================================================================
XMINT2 Enemy::SetStartIndex() {
	return mapIndex;
}

//====================================================================================
//
//				�g�p�t���O�̎擾
//
//====================================================================================
bool Enemy::GetUse() {
	return use;
}