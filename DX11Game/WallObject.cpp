//************************************************************************************
//
// �ǃI�u�W�F�N�g [WallObject.cpp]
// Author : �ɒn�c�^��
//
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "WallObject.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "player.h"
#include "GameObject.h"
#include "DebugCollision.h"

//-------------------- �}�N����` --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define	WALLOBJ_RADIUS		(45.0f)		// ���E�����a
#define MAPNUM				(16)		// �ǂ�u����c�����ʂ̍ő吔

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;		// ���f��

//static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
//static XMFLOAT3		g_moveModel;	// �ړ���
//static XMFLOAT3		g_size;			// ���f���̕`��T�C�Y

//static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

static int			g_nShadow;		// �ۉe�ԍ�

GameObject *g_wallobj[MAPNUM*MAPNUM];

int g_wallMap[16][16] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT InitWallObj(void) {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int j = 0; j < MAPNUM; j++) {
		for (int i = 0; i < MAPNUM; i++) {
			g_wallobj[j*MAPNUM+i] = new GameObject;
			g_wallobj[j*MAPNUM+i]->m_size = XMFLOAT3(25.0f, 25.0f, 25.0f);
			g_wallobj[j*MAPNUM+i]->m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_wallobj[j*MAPNUM+i]->m_radius = WALLOBJ_RADIUS;

			if (g_wallMap[j][i] != 1) {
				continue;
			}
			g_wallobj[j*MAPNUM+i]->m_pos = XMFLOAT3(i*80.0f - 640.0f, 0.0f, j*80.0f - 640.0f);
		}
	}

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_size = XMFLOAT3(25.0f, 25.0f, 25.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}
	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitWallObj(void) {
	// �ǃI�u�W�F�N�g�̉��
	for (int j = 0; j < MAPNUM; j++) {
		for (int i = 0; i < MAPNUM; i++) {
			delete g_wallobj[j * MAPNUM + i];
		}
	}

	// ���f���̉��
	g_model.Release();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void UpdateWallObj(void) {
	//if (GetKeyPress(VK_RETURN)) {
	//	// ���Z�b�g
	//	g_posModel = XMFLOAT3(0.0f, 40.0f, 0.0f);
	//	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	for (int j = 0; j < MAPNUM; j++) {
		for (int i = 0; i < MAPNUM; i++) {
			if (g_wallMap[j][i] != 1) {
				continue;
			}
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(
				g_wallobj[j * MAPNUM + i]->m_size.x,
				g_wallobj[j * MAPNUM + i]->m_size.y,
				g_wallobj[j * MAPNUM + i]->m_size.z);

			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
				XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_wallobj[j * MAPNUM + i]->m_pos.x,
				g_wallobj[j * MAPNUM + i]->m_pos.y,
				g_wallobj[j * MAPNUM + i]->m_pos.z);

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_wallobj[j * MAPNUM + i]->m_mtxWorld, mtxWorld);
		}
	}

	// �����蔻��
	CollisionWallObj();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawWallObj(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int j = 0; j < MAPNUM; j++) {
		for (int i = 0; i < MAPNUM; i++) {
			if (g_wallMap[j][i] != 1) {
				continue;
			}

			// �s����������`��
			g_model.Draw(pDC, g_wallobj[j * MAPNUM + i]->m_mtxWorld, eOpacityOnly);

			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
			g_model.Draw(pDC, g_wallobj[j * MAPNUM + i]->m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����

			// �f�o�b�O�\��
			DrawCollisionSphere(*g_wallobj[j * MAPNUM + i]);
		}
	}
}

//====================================================================================
//
//				�ʒu�擾
//
//====================================================================================
//XMFLOAT3& GetWallObjPos() {
//	return g_posModel;
//}

//====================================================================================
//
//				�Փ˔���
//
//====================================================================================
void CollisionWallObj() {
	bool isHit;
	for (int j = 0; j < MAPNUM; j++) {
		for (int i = 0; i < MAPNUM; i++) {
			if (g_wallMap[j][i] != 1) {
				continue;
			}

			//isHit = CollisionPlayer(g_wallobj[j * MAPNUM + i]->m_pos, g_wallobj[j * MAPNUM + i]->m_radius, g_wallobj[j * MAPNUM + i]->m_size);
			isHit = CollisionPlayer(*g_wallobj[j * MAPNUM + i]);
			if (isHit) {
				return;
			}
		}
	}
}
