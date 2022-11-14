//************************************************************************************
// 
// �G�@[enemy.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "enemy.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "collision.h"
#include "explosion.h"
#include "DwarfEffect.h"
#include "Sound.h"
#include "bullet.h"
#include "GameObject.h"
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
//static TEnemy		g_enemy[MAX_ENEMY];	// �G�@���
static int			g_enemyKillSum;		// ���j��
//static GameObject	*g_GameObj[MAX_ENEMY];

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
void Enemy::Init()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	g_enemyKillSum = 0;

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
	}

	//for (int i = 0; i < MAX_ENEMY; ++i) {
	//	g_GameObj[i] = new GameObject;	// �C���X�^���X��

	//	// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
	//	g_GameObj[i]->pos = XMFLOAT3(rand() % 620 - 310.0f,
	//								0.0f,
	//								rand() % 620 - 310.0f);
	//	g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	//	g_enemy[i].m_rotDest = g_enemy[i].m_rot;
	//	g_GameObj[i]->moveVal = XMFLOAT3(
	//		-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
	//		0.0f,
	//		-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);
	//	g_GameObj[i]->size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	//	// �ۉe�̐���
	//	g_enemy[i].m_nShadow = CreateShadow(g_GameObj[i]->pos, 25.0f);
	//	g_enemy[i].m_use = true;
	//	g_enemy[i].m_bulletTimer = 300;
	//}

	// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
	pos = XMFLOAT3(rand() % 620 - 310.0f,0.0f,	rand() % 620 - 310.0f);
	rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rot;
	moveVal = XMFLOAT3(
	-SinDeg(rot.y) * VALUE_MOVE_ENEMY,
	0.0f,
	-CosDeg(rot.y) * VALUE_MOVE_ENEMY);
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	// �ۉe�̐���
	shadowNum = CreateShadow(pos, 25.0f);
	use = true;
	bulletTimer = 300;

}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Enemy::Uninit()
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		//delete g_GameObj[i];
		// �ۉe�̉��
		ReleaseShadow(shadowNum);
	}

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Enemy::Update()
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate ,mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f,0.0f,0.0f);

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!use) {
			continue;
		}
		g_GameObj[i]->moveVal = XMFLOAT3(
			-SinDeg(rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(rot.y) * VALUE_MOVE_ENEMY);

		if (g_GameObj[i]->moveVal.x > 0.0f) {
			ShadowMove.x += 25.0f;
		} else if (g_GameObj[i]->moveVal.x < 0.0f) {
			ShadowMove.x -= 25.0f;
		}

		if (g_GameObj[i]->moveVal.z > 0.0f) {
			ShadowMove.z += 25.0f;
		} else if (g_GameObj[i]->moveVal.z < 0.0f) {
			ShadowMove.z -= 25.0f;
		}


		// �ړ�
		g_GameObj[i]->pos.x += g_GameObj[i]->moveVal.x;
		g_GameObj[i]->pos.y += g_GameObj[i]->moveVal.y;
		g_GameObj[i]->pos.z += g_GameObj[i]->moveVal.z;

		// �ǂɂԂ�����
		bool lr = false, fb = false;
		if (g_GameObj[i]->pos.x < -310.0f) {
			g_GameObj[i]->pos.x = -310.0f;
			lr = true;
		}
		if (g_GameObj[i]->pos.x > 310.0f) {
			g_GameObj[i]->pos.x = 310.0f;
			lr = true;
		}
		if (g_GameObj[i]->pos.z < -310.0f) {
			g_GameObj[i]->pos.z = -310.0f;
			fb = true;
		}
		if (g_GameObj[i]->pos.z > 310.0f) {
			g_GameObj[i]->pos.z = 310.0f;
			fb = true;
		}
		if (g_GameObj[i]->pos.y < 0.0f) {
			g_GameObj[i]->pos.y = 0.0f;
		}
		if (g_GameObj[i]->pos.y > 80.0f) {
			g_GameObj[i]->pos.y = 80.0f;
		}
		if (fabsf(g_enemy[i].m_rot.y - g_enemy[i].m_rotDest.y) < 0.0001f) {
			if (lr) {
				g_GameObj[i]->moveVal.x *= -1.0f;
			}
			if (fb) {
				g_GameObj[i]->moveVal.z *= -1.0f;
			}
			if (lr || fb) {
				g_enemy[i].m_rotDest.y = XMConvertToDegrees(atan2f(-g_GameObj[i]->moveVal.x, -g_GameObj[i]->moveVal.z));
			}
		}

		// �ړI�̊p�x�܂ł̍���
		float fDiffRotY = g_enemy[i].m_rotDest.y - g_enemy[i].m_rot.y;
		if (fDiffRotY >= 180.0f) {
			fDiffRotY -= 360.0f;
		}
		if (fDiffRotY < -180.0f) {
			fDiffRotY += 360.0f;
		}

		// �ړI�̊p�x�܂Ŋ�����������
		g_enemy[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
		if (g_enemy[i].m_rot.y >= 180.0f) {
			g_enemy[i].m_rot.y -= 360.0f;
		}
		if (g_enemy[i].m_rot.y < -180.0f) {
			g_enemy[i].m_rot.y += 360.0f;
		}


		// �}�b�v�̗v�f�ԍ��ł������猻�݂̈ʒu���ǂ��ɂȂ�̂������߂�
		g_GameObj[i]->mapIndex.x = (g_GameObj[i]->pos.x + 640.0f) / 80.0f;
		g_GameObj[i]->mapIndex.y = abs(g_GameObj[i]->pos.z - 480.0) / 80.0f;


		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		/* ���[���h���W�̕ϊ��͕K���A�g��A��]�A�ړ��̏��ōs�� */

		// �X�P�[���𔽉f
		mtxScale = XMMatrixScaling(g_GameObj[i]->size.x, g_GameObj[i]->size.y, g_GameObj[i]->size.z);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);


		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemy[i].m_rot.x),
			XMConvertToRadians(g_enemy[i].m_rot.y),
			XMConvertToRadians(g_enemy[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_GameObj[i]->pos.x,
			g_GameObj[i]->pos.y,
			g_GameObj[i]->pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_GameObj[i]->mtxWorld, mtxWorld);

		// �ۉe�̈ړ�
		MoveShadow(g_enemy[i].m_nShadow, XMFLOAT3(g_GameObj[i]->pos.x+ShadowMove.x, g_GameObj[i]->pos.y, g_GameObj[i]->pos.z+ShadowMove.z));

		int randomtime;
		randomtime = rand() % 3;
		g_enemy[i].m_bulletTimer -= randomtime;
		if (g_enemy[i].m_bulletTimer < 0) {
			FireBullet(
				g_GameObj[i]->pos,
				XMFLOAT3(-g_GameObj[i]->mtxWorld._31, -g_GameObj[i]->mtxWorld._32, -g_GameObj[i]->mtxWorld._33),
				BULLETTYPE_ENEMY);

			g_enemy[i].m_bulletTimer = BULLET_TIME;
		}
		
		//g_enemy[i].m_rootTimer--;
		//if (g_enemy[i].m_rootTimer < 0) {
		//	g_enemy[i].m_mapIndex = search_Root(i);
		//	g_enemy[i].m_rootTimer = ROOT_TIME;
		//}
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Enemy::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		g_model.Draw(pDC, g_GameObj[i]->mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_GameObj[i]->mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//====================================================================================
//
//				�����蔻��
//
//====================================================================================
int CollisionEnemy(XMFLOAT3 pos, float radius, float damage) {
	int hitNum = 0;
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		bool hit = CollisionSphere(g_GameObj[i]->pos, ENEMY_RADIUS, pos, radius);
		if (hit) {
			hitNum++;
			// �����J�n
			int nExp = -1;
			if (damage > 0.0f) {
				StartDwarfEffect(g_GameObj[i]->pos);
				//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
				 
			} else {
				nExp = StartExplosion(g_GameObj[i]->pos, XMFLOAT2(20.0f, 20.0f));
			}
			SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));

			g_enemyKillSum++;
			g_enemy[i].m_use = false;
			ReleaseShadow(g_enemy[i].m_nShadow);
			CSound::Play(SE_KILL);
		}
	}
	return hitNum;
}

void CollisionEnemy(GameObject collision) {
	GameObject* _GameObject;

	for (int i = 0; i < MAX_ENEMY; i++) {
		bool isHit = CollisionSphere(*g_GameObj[i], collision);

		if (isHit) {
			_GameObject = Push(g_GameObj[i]->pos, XMFLOAT3(10.0f,10.0f,10.0f), g_GameObj[i]->moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));

			if (_GameObject == nullptr) {
				//�@�����o���Ɏ��s���Ă���
				return;
			}

			g_GameObj[i]->pos = _GameObject->pos;
			g_GameObj[i]->moveVal = _GameObject->moveVal;
		}
	}
}

//====================================================================================
//
//				���j���擾
//
//====================================================================================
int GetEnemyKillSum() {
	return g_enemyKillSum;
}


//====================================================================================
//
//				���Ɉړ�����ׂ��ꏊ���i�[����index��ۑ�����
//
//====================================================================================
void SetRootIndex(XMINT2* index,int enemyNo) {
	g_enemy[enemyNo].m_mapIndex = index;
}

//====================================================================================
//
//				A*�̊J�n�n�_�������
//
//====================================================================================
XMINT2 SetStartIndex(int enemyNo) {
	return g_GameObj[enemyNo]->mapIndex;
}