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

//-------------------- �萔��` --------------------
#define MODEL_ENEMY			"data/model/kobitored.fbx"

#define	VALUE_MOVE_ENEMY	(0.40f)		// �ړ����x
#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��
#define ENEMY_RADIUS		(50.0f)		// ���E�����a


//-------------------- �\���̒�` --------------------
struct TEnemy {
	XMFLOAT3	m_pos;			// ���݂̈ʒu
	XMFLOAT3	m_rot;			// ���݂̌���
	XMFLOAT3	m_rotDest;		// �ړI�̌���
	XMFLOAT3	m_move;			// �ړ���
	XMFLOAT3	m_size;			// �傫��
	bool		m_use;			// �g�p�����ǂ���
	int			m_bulletTimer;	// �e���˃^�C�}�[

	XMFLOAT4X4	m_mtxWorld;		// ���[���h�}�g���b�N�X

	int			m_nShadow;		// �ۉe�ԍ�
};

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;			// ���f��
static TEnemy		g_enemy[MAX_ENEMY];	// �G�@���
static int			g_enemyKillSum;		// ���j��

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	g_enemyKillSum = 0;

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
		g_enemy[i].m_pos = XMFLOAT3(rand() % 620 - 310.0f,
									0.0f,
									rand() % 620 - 310.0f);
		g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_enemy[i].m_rotDest = g_enemy[i].m_rot;
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);
		g_enemy[i].m_size = XMFLOAT3(50.0f, 50.0f, 50.0f);
		// �ۉe�̐���
		g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 25.0f);
		g_enemy[i].m_use = true;
		g_enemy[i].m_bulletTimer = 300;
	}

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ۉe�̉��
		ReleaseShadow(g_enemy[i].m_nShadow);
	}

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void UpdateEnemy(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate ,mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f,0.0f,0.0f);

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		// �ړ�
		g_enemy[i].m_pos.x += g_enemy[i].m_move.x;
		g_enemy[i].m_pos.y += g_enemy[i].m_move.y;
		g_enemy[i].m_pos.z += g_enemy[i].m_move.z;

		// �ǂɂԂ�����
		bool lr = false, fb = false;
		if (g_enemy[i].m_pos.x < -310.0f) {
			g_enemy[i].m_pos.x = -310.0f;
			lr = true;
		}
		if (g_enemy[i].m_pos.x > 310.0f) {
			g_enemy[i].m_pos.x = 310.0f;
			lr = true;
		}
		if (g_enemy[i].m_pos.z < -310.0f) {
			g_enemy[i].m_pos.z = -310.0f;
			fb = true;
		}
		if (g_enemy[i].m_pos.z > 310.0f) {
			g_enemy[i].m_pos.z = 310.0f;
			fb = true;
		}
		if (g_enemy[i].m_pos.y < 0.0f) {
			g_enemy[i].m_pos.y = 0.0f;
		}
		if (g_enemy[i].m_pos.y > 80.0f) {
			g_enemy[i].m_pos.y = 80.0f;
		}
		if (fabsf(g_enemy[i].m_rot.y - g_enemy[i].m_rotDest.y) < 0.0001f) {
			if (lr) {
				g_enemy[i].m_move.x *= -1.0f;
			}
			if (fb) {
				g_enemy[i].m_move.z *= -1.0f;
			}
			if (lr || fb) {
				g_enemy[i].m_rotDest.y = XMConvertToDegrees(atan2f(-g_enemy[i].m_move.x, -g_enemy[i].m_move.z));
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
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);

		if (g_enemy[i].m_move.x > 0.0f) {
			ShadowMove.x += 25.0f;
		} else if (g_enemy[i].m_move.x < 0.0f) {
			ShadowMove.x -= 25.0f;
		}

		if (g_enemy[i].m_move.z > 0.0f) {
			ShadowMove.z += 25.0f;
		} else if (g_enemy[i].m_move.z < 0.0f) {
			ShadowMove.z -= 25.0f;
		}


		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		/* ���[���h���W�̕ϊ��͕K���A�g��A��]�A�ړ��̏��ōs�� */

		// �X�P�[���𔽉f
		mtxScale = XMMatrixScaling(g_enemy[i].m_size.x, g_enemy[i].m_size.y, g_enemy[i].m_size.z);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);


		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemy[i].m_rot.x),
			XMConvertToRadians(g_enemy[i].m_rot.y),
			XMConvertToRadians(g_enemy[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);

		// �ۉe�̈ړ�
		MoveShadow(g_enemy[i].m_nShadow, XMFLOAT3(g_enemy[i].m_pos.x+ShadowMove.x,g_enemy[i].m_pos.y,g_enemy[i].m_pos.z+ShadowMove.z));

		int randomtime;
		randomtime = rand() % 3;
		g_enemy[i].m_bulletTimer -= randomtime;
		if (g_enemy[i].m_bulletTimer < 0) {
			FireBullet(
				g_enemy[i].m_pos,
				XMFLOAT3(-g_enemy[i].m_mtxWorld._31, -g_enemy[i].m_mtxWorld._32, -g_enemy[i].m_mtxWorld._33),
				BULLETTYPE_ENEMY);

			g_enemy[i].m_bulletTimer = 300;
		}
		
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eTransparentOnly);
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
		bool hit = CollisionSphere(g_enemy[i].m_pos, ENEMY_RADIUS, pos, radius);
		if (hit) {
			hitNum++;
			// �����J�n
			int nExp = -1;
			if (damage > 0.0f) {
				StartDwarfEffect(g_enemy[i].m_pos);
				//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
				 
			} else {
				nExp = StartExplosion(g_enemy[i].m_pos, XMFLOAT2(20.0f, 20.0f));
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

//====================================================================================
//
//				���j���擾
//
//====================================================================================
int GetEnemyKillSum() {
	return g_enemyKillSum;
}