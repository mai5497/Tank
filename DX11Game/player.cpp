//************************************************************************************
//
// ���@���� [player.cpp]
// Author : �ɒn�c�^��
//
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "player.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"

//-------------------- �}�N����` --------------------
#define MODEL_PLAYER		"data/model/kobitoblue.fbx"

#define	VALUE_MOVE_PLAYER	(0.155f)	// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)	// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��

#define	PLAYER_RADIUS		(10.0f)		// ���E�����a

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;		// ���f��

static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
static XMFLOAT3		g_moveModel;	// �ړ���
static XMFLOAT3		g_size;			// ���f���̕`��T�C�Y

static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

static int			g_nShadow;		// �ۉe�ԍ�

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT InitPlayer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_size = XMFLOAT3(50.0f, 50.0f, 50.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}

	// �ۉe�̐���
	g_nShadow = CreateShadow(g_posModel, 12.0f);

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitPlayer(void)
{
	// �ۉe�̉��
	ReleaseShadow(g_nShadow);

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void UpdatePlayer(void)
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (GetKeyPress(VK_A)) {
		if (GetKeyPress(VK_W)) {
			// ���O�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// ����ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// ���ړ�
			g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_D)) {
		if (GetKeyPress(VK_W)) {
			// �E�O�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// �E��ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// �E�ړ�
			g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;

			g_rotDestModel.y = rotCamera.y - 90.0f;
		}
	} else if (GetKeyPress(VK_W)) {
		// �O�ړ�
		g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_S)) {
		// ��ړ�
		g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_J)) {
		// ����]
		g_rotDestModel.y -= VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_L)) {
		// �E��]
		g_rotDestModel.y += VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	/// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	// �ړ��ʂɊ�����������
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_PLAYER;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_PLAYER;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_PLAYER;

	if (g_posModel.x < -630.0f) {
		g_posModel.x = -630.0f;
	}
	if (g_posModel.x > 630.0f) {
		g_posModel.x = 630.0f;
	}
	if (g_posModel.z < -630.0f) {
		g_posModel.z = -630.0f;
	}
	if (g_posModel.z > 630.0f) {
		g_posModel.z = 630.0f;
	}
	if (g_posModel.y < 10.0f) {
		g_posModel.y = 10.0f;
	}
	if (g_posModel.y > 150.0f) {
		g_posModel.y = 150.0f;
	}

	//if (GetKeyPress(VK_RETURN)) {
	//	// ���Z�b�g
	//	g_posModel = XMFLOAT3(0.0f, 40.0f, 0.0f);
	//	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(g_size.x, g_size.y, g_size.z);
	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	MoveShadow(g_nShadow, g_posModel);

	if ((g_moveModel.x * g_moveModel.x
		+ g_moveModel.y * g_moveModel.y
		+ g_moveModel.z * g_moveModel.z) > 1.0f) {
		XMFLOAT3 pos;

		pos.x = g_posModel.x + SinDeg(g_rotModel.y) * 10.0f;
		pos.y = g_posModel.y + 2.0f;
		pos.z = g_posModel.z + CosDeg(g_rotModel.y) * 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.85f, 0.05f, 0.65f, 0.50f),
			XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f),
			XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
			XMFLOAT2(5.0f, 5.0f), 20);
	}

	// �e����
	if (GetKeyRepeat(VK_SPACE)) {
		FireBullet(g_posModel, XMFLOAT3(-g_mtxWorld._31, -g_mtxWorld._32, -g_mtxWorld._33),
			BULLETTYPE_PLAYER);
	}

	//PrintDebugProc("[˺�� �� : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	//PrintDebugProc("[˺�� ѷ : (%f) < Ӹ÷ ��:(%f) >]\n", g_rotModel.y, g_rotDestModel.y);
	//PrintDebugProc("\n");

	//PrintDebugProc("*** ˺�� ��� ***\n");
	//PrintDebugProc("ϴ   ��޳ : \x1e\n");//��
	//PrintDebugProc("���  ��޳ : \x1f\n");//��
	//PrintDebugProc("���� ��޳ : \x1d\n");//��
	//PrintDebugProc("з�  ��޳ : \x1c\n");//��
	//PrintDebugProc("�ޮ����   : I\n");
	//PrintDebugProc("���       : K\n");
	//PrintDebugProc("���� �ݶ� : J\n");
	//PrintDebugProc("з�  �ݶ� : L\n");
	//PrintDebugProc("��   ʯ�� : Space\n");
	//PrintDebugProc("\n");
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawPlayer(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//====================================================================================
//
//				�ʒu�擾
//
//====================================================================================
XMFLOAT3& GetPlayerPos()
{
	return g_posModel;
}

//====================================================================================
//
//				�Փ˔���
//
//====================================================================================
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage)
{
	bool hit = CollisionSphere(g_posModel, PLAYER_RADIUS, pos, radius);
	if (hit) {
		// �����J�n
		int nExp = -1;
		if (damage > 0.0f) {
			nExp = StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			// 
		} else {
			nExp = StartExplosion(g_posModel, XMFLOAT2(20.0f, 20.0f));
		}
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
	}
	return hit;
}
