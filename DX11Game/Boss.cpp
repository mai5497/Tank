//************************************************************************************
// 
// �{�X[Bossp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "Boss.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "collision.h"
#include "explosion.h"
#include "DwarfEffect.h"
#include "Sound.h"

//-------------------- �萔��` --------------------
#define MODEL_BOSS			"data/model/kobitored.fbx"

#define	VALUE_MOVE_BOSS	(0.40f)		// �ړ����x
#define	RATE_MOVE_BOSS		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_BOSS	(7.0f)		// ��]���x
#define	RATE_ROTATE_BOSS	(0.20f)		// ��]�����W��
#define BOSS_RADIUS		(200.0f)		// ���E�����a


//-------------------- �\���̒�` --------------------
struct TBoss {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	XMFLOAT3	m_size;		// �傫��
	bool		m_use;		// �g�p�����ǂ���
	int			m_HP;		// HP


	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X

	int			m_nShadow;	// �ۉe�ԍ�
};

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;			// ���f��
static TBoss		g_boss;	// �G�@���

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT InitBoss(void) {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_BOSS)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitBoss", MB_OK);
		return E_FAIL;
	}

	// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
	g_boss.m_pos = XMFLOAT3(rand() % 620 - 310.0f,
		0.0f,
		rand() % 620 - 310.0f);
	g_boss.m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	g_boss.m_rotDest = g_boss.m_rot;
	g_boss.m_move = XMFLOAT3(
		-SinDeg(g_boss.m_rot.y) * VALUE_MOVE_BOSS,
		0.0f,
		-CosDeg(g_boss.m_rot.y) * VALUE_MOVE_BOSS);
	g_boss.m_size = XMFLOAT3(200.0f, 200.0f, 200.0f);
	// �ۉe�̐���
	g_boss.m_nShadow = CreateShadow(g_boss.m_pos, 12.0f);
	g_boss.m_use = true;
	g_boss.m_HP = 10;

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitBoss(void) {
	// �ۉe�̉��
	ReleaseShadow(g_boss.m_nShadow);
	

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
bool UpdateBoss(void) {
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	if (!g_boss.m_use) {
		return false;
	}
	// �ړ�
	g_boss.m_pos.x += g_boss.m_move.x;
	g_boss.m_pos.y += g_boss.m_move.y;
	g_boss.m_pos.z += g_boss.m_move.z;

	// �ǂɂԂ�����
	bool lr = false, fb = false;
	if (g_boss.m_pos.x < -310.0f) {
		g_boss.m_pos.x = -310.0f;
		lr = true;
	}
	if (g_boss.m_pos.x > 310.0f) {
		g_boss.m_pos.x = 310.0f;
		lr = true;
	}
	if (g_boss.m_pos.z < -310.0f) {
		g_boss.m_pos.z = -310.0f;
		fb = true;
	}
	if (g_boss.m_pos.z > 310.0f) {
		g_boss.m_pos.z = 310.0f;
		fb = true;
	}
	if (g_boss.m_pos.y < 0.0f) {
		g_boss.m_pos.y = 0.0f;
	}
	if (g_boss.m_pos.y > 80.0f) {
		g_boss.m_pos.y = 80.0f;
	}
	if (fabsf(g_boss.m_rot.y - g_boss.m_rotDest.y) < 0.0001f) {
		if (lr) {
			g_boss.m_move.x *= -1.0f;
		}
		if (fb) {
			g_boss.m_move.z *= -1.0f;
		}
		if (lr || fb) {
			g_boss.m_rotDest.y = XMConvertToDegrees(atan2f(-g_boss.m_move.x, -g_boss.m_move.z));
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = g_boss.m_rotDest.y - g_boss.m_rot.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_boss.m_rot.y += fDiffRotY * RATE_ROTATE_BOSS;
	if (g_boss.m_rot.y >= 180.0f) {
		g_boss.m_rot.y -= 360.0f;
	}
	if (g_boss.m_rot.y < -180.0f) {
		g_boss.m_rot.y += 360.0f;
	}
	g_boss.m_move = XMFLOAT3(
		-SinDeg(g_boss.m_rot.y) * VALUE_MOVE_BOSS,
		0.0f,
		-CosDeg(g_boss.m_rot.y) * VALUE_MOVE_BOSS);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	/* ���[���h���W�̕ϊ��͕K���A�g��A��]�A�ړ��̏��ōs�� */

	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(g_boss.m_size.x, g_boss.m_size.y, g_boss.m_size.z);
	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);


	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(g_boss.m_rot.x),
		XMConvertToRadians(g_boss.m_rot.y),
		XMConvertToRadians(g_boss.m_rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(
		g_boss.m_pos.x,
		g_boss.m_pos.y,
		g_boss.m_pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_boss.m_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	MoveShadow(g_boss.m_nShadow, g_boss.m_pos);

	return true;
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawBoss(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_boss.m_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	g_model.Draw(pDC, g_boss.m_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//====================================================================================
//
//				�����蔻��
//
//====================================================================================
int CollisionBoss(XMFLOAT3 pos, float radius, float damage) {
	int hitNum = 0;
	bool hit = CollisionSphere(g_boss.m_pos, BOSS_RADIUS, pos, radius);
	if (hit) {
		hitNum++;
		// �����J�n
		int nExp = -1;
		if (damage > 0.0f) {
			g_boss.m_HP--;
			StartDwarfEffect(g_boss.m_pos);
			//nExp = SetEffect(g_boss[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);

		} else {
			nExp = StartExplosion(g_boss.m_pos, XMFLOAT2(20.0f, 20.0f));
		}
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));

		if (g_boss.m_HP < 0) {	// �ڂ�����
			g_boss.m_use = false;
			ReleaseShadow(g_boss.m_nShadow);
			CSound::Play(SE_KILL);
		}
	}
	return hitNum;
}

