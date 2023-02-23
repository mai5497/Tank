//************************************************************************************
//
// �ǃI�u�W�F�N�g [WallObject.cpp]
// Author : �ɒn�c�^��
//
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "WallObject.h"
#include "input.h"
#include "debugproc.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "player.h"
#include "DebugCollision.h"
#include "enemy.h"
#include "Astar.h"
#include "DwarfEffect.h"
#include "Texture.h"

//-------------------- �}�N����` --------------------
#define MODEL_WALLOBJ		"data/model/rubikcube.fbx"
#define BUMP_WALLOBJ		"data/model/rubikcube_bump.png"
#define	WALLOBJ_RADIUS		(55.0f)		// ���E�����a

std::unique_ptr<CAssimpModel> WallObj::pMyModel;
//int WallObj::wallMap[MAPHEIGHT][MAPWIDTH] = {
//	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//};

int WallObj::wallMap[MAPHEIGHT][MAPWIDTH] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};


//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
WallObj::WallObj() {

}

WallObj::WallObj(int x, int y) {
	mapIndex.x = x;
	mapIndex.y = y;
}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
WallObj::~WallObj() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void WallObj::Init() {
	// ���f���f�[�^�̓ǂݍ���
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

		// �o���v�}�b�v�Ǎ�
		//std::unique_ptr<Texture> _pTexture = std::make_unique<Texture>();		// �e�N�X�`��
		//_pTexture->SetTexture(pDevice, BUMP_WALLOBJ);
		//pMyModel->SetShaderMode(CAssimpModel::SM_BUMP);	// �o���v�}�b�v����		
		//pMyModel->SetShaderMat(_pTexture->GetTexture());
		////_pTexture->ReleaseTexture();
		//_pTexture.reset();

		//if (_pTexture) {
		//	pDeviceContext->PSSetShaderResources(4, 1, &_pTexture);
		//} else {
		//	MessageBoxA(GetMainWnd(), "�o���v�}�b�v�摜�ǂݍ��݃G���[", "InitWallObj", MB_OK);
		//}

		// ���f���f�[�^�Ǎ�
		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_WALLOBJ)) {
			MessageBoxA(GetMainWnd(), "�ǃ��f���f�[�^�ǂݍ��݃G���[", "InitWallObj", MB_OK);
		}

		// �}�b�v�̐ݒ����x�s���΂悢���߃��f���Ǎ��Ɠ��������Ƃ�
		int* pMap = (int*)wallMap;
		SetMap(pMap);
	}

	if (wallMap[mapIndex.y][mapIndex.x] != 0) {
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		size = XMFLOAT3(25.0f, 25.0f, 25.0f);
		moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		collRadius = WALLOBJ_RADIUS;
		collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);

		rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

		isCollision = true;

		XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;

		pos = XMFLOAT3(mapIndex.x * 80.0f - 640.0f, 0.0f, -mapIndex.y * 80.0f + 480.0f);
		// ���[���h�}�g���b�N�X�̏�����
		_mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
		_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);

		// ��]�𔽉f
		_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotModel.x), XMConvertToRadians(rotModel.y), XMConvertToRadians(rotModel.z));
		_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

		// �ړ��𔽉f
		_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
		_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&mtxWorld, _mtxWorld);
	}
	collType = Collision::STATIC;
	myTag = WALL;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void WallObj::Uninit() {
	// ���f���̉��
	if (pMyModel) {
		pMyModel->Release();
		pMyModel.reset();
	}
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void WallObj::Update() {
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void WallObj::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	if (wallMap[mapIndex.y][mapIndex.x] == 0) {
		return;
	}

	// �s����������`��
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

	// �f�o�b�O�\��
	//DrawCollisionSphere(this);
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
//void CollisionWallObj() {
//	//bool isHit;
//	for (int j = 0; j < MAPHEIGHT; j++) {
//		for (int i = 0; i < MAPWIDTH; i++) {
//			if (g_wallMap[j][i] == 0) {
//				continue;
//			}
//
//			//isHit = CollisionPlayer(g_wallobj[j * MAPNUM + i]->m_pos, g_wallobj[j * MAPNUM + i]->m_radius, g_wallobj[j * MAPNUM + i]->m_size);
//			
//			/*
//			* ���ƂŃI�u�W�F�N�g���m�̎󂯓n�������I�I�I�I�I�I
//			*/
//			
//			
//			//CollisionPlayer(*g_wallobj[j * MAPWIDTH + i]);
//			//CollisionEnemy(*g_wallobj[j * MAPWIDTH + i]);
//			
//			
//			
//			
//			
//			//if (isHit) {
//			//	return;
//			//}
//		}
//	}
//}

/// <summary>
/// �e�ƕǂ̓����蔻��
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
/// <param name="damage"></param>
/// <returns></returns>
//bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage) {
//	bool hit;
//	Collision _collision;
//	for (int j = 0; j < MAPHEIGHT; j++) {
//		for (int i = 0; i < MAPWIDTH; i++) {
//			if (g_wallMap[j][i] == 0) {
//				continue;
//			}
//			hit = _collision.CollisionSphere(g_wallobj[j][i].pos, 45, pos, radius);
//			if (hit) {
//				// �����J�n
//				int nExp = -1;
//				if (damage > 0.0f) {
//					StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(40.0f, 40.0f));
//					//nExp = SetEffect(g_enemy[i].m_pos, XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
//
//				} else {
//					nExp = StartExplosion(g_wallobj[j][i].pos, XMFLOAT2(20.0f, 20.0f));
//				}
//				SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
//			}
//		}
//	}
//	return hit;
//}

