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
#include "enemy.h"
#include "Astar.h"
#include "DwarfEffect.h"

//-------------------- �}�N����` --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define	WALLOBJ_RADIUS		(55.0f)		// ���E�����a

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;		// ���f��

//static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
//static XMFLOAT3		g_moveModel;	// �ړ���
//static XMFLOAT3		g_size;			// ���f���̕`��T�C�Y

//static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

static int			g_nShadow;		// �ۉe�ԍ�

GameObject *g_wallobj[MAPWIDTH * MAPHEIGHT];

int g_wallMap[MAPHEIGHT][MAPWIDTH] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
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

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			g_wallobj[j* MAPWIDTH +i] = new GameObject;
			g_wallobj[j* MAPWIDTH +i]->size = XMFLOAT3(25.0f, 25.0f, 25.0f);
			g_wallobj[j* MAPWIDTH +i]->moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_wallobj[j* MAPWIDTH +i]->collRadius = WALLOBJ_RADIUS;

			if (g_wallMap[j][i] == 0) {
				continue;
			}
			g_wallobj[j* MAPWIDTH +i]->pos = XMFLOAT3(i*80.0f - 640.0f, 0.0f, -j*80.0f + 480.0f);
			g_wallobj[j * MAPWIDTH + i]->mapIndex.x = i;
			g_wallobj[j * MAPWIDTH + i]->mapIndex.y = j;
		}
	}

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_size = XMFLOAT3(25.0f, 25.0f, 25.0f);

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(
				g_wallobj[j * MAPWIDTH + i]->size.x,
				g_wallobj[j * MAPWIDTH + i]->size.y,
				g_wallobj[j * MAPWIDTH + i]->size.z);

			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
				XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_wallobj[j * MAPWIDTH + i]->pos.x,
				g_wallobj[j * MAPWIDTH + i]->pos.y,
				g_wallobj[j * MAPWIDTH + i]->pos.z);

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_wallobj[j * MAPWIDTH + i]->mtxWorld, mtxWorld);
		}
	}


	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}

	int* pMap = (int*)g_wallMap;
	SetMap(pMap);

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitWallObj(void) {
	// �ǃI�u�W�F�N�g�̉��
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			delete g_wallobj[j * MAPWIDTH + i];
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

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}

			// �s����������`��
			g_model.Draw(pDC, g_wallobj[j * MAPWIDTH + i]->mtxWorld, eOpacityOnly);

			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
			g_model.Draw(pDC, g_wallobj[j * MAPWIDTH + i]->mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����

			// �f�o�b�O�\��
			DrawCollisionSphere(*g_wallobj[j * MAPWIDTH + i]);
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
	//bool isHit;
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}

			//isHit = CollisionPlayer(g_wallobj[j * MAPNUM + i]->m_pos, g_wallobj[j * MAPNUM + i]->m_radius, g_wallobj[j * MAPNUM + i]->m_size);
			CollisionPlayer(*g_wallobj[j * MAPWIDTH + i]);
			CollisionEnemy(*g_wallobj[j * MAPWIDTH + i]);
			//if (isHit) {
			//	return;
			//}
		}
	}
}

/// <summary>
/// �e�ƕǂ̓����蔻��
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
/// <param name="damage"></param>
/// <returns></returns>
bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage) {
	bool hit;
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			if (g_wallMap[j][i] == 0) {
				continue;
			}
			hit = CollisionSphere(g_wallobj[j][i].pos, 45, pos, radius);
			if (hit) {
				// �����J�n
				int nExp = -1;
				if (damage > 0.0f) {
					StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(40.0f, 40.0f));
					//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);

				} else {
					nExp = StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(20.0f, 20.0f));
				}
				SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
			}
		}
	}
	return hit;
}

