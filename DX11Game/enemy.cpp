//************************************************************************************
// 
// �G[enemy.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "Enemy.h"
#include "debugproc.h"
#include "shadow.h"
#include "explosion.h"
#include "DwarfEffect.h"
#include "Sound.h"
#include "bullet.h"
#include "collision.h"
#include "Astar.h"
#include "GameObjManager.h"

//-------------------- �萔��` --------------------
#define MODEL_ENEMY			"data/model/kobitored.fbx"

#define	VALUE_MOVE_ENEMY	(0.40f)		// �ړ����x
#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��
#define ENEMY_RADIUS		(50.0f)		// ���E�����a

#define BULLET_TIME			(300)		// �e���˂܂ł̎���
#define ROOT_TIME			(300)		// ���Ƀ��[�g��������܂ł̎���


std::unique_ptr<CAssimpModel> Enemy::pMyModel;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Enemy::Enemy() {
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
	// ���f���f�[�^�̓ǂݍ���
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		}
	}

	// �ʒu�E��]�E�X�P�[���E�T�C�Y�̏����ݒ�
	pos = XMFLOAT3(rand() % 620 - 310.0f, 0.0f, rand() % 620 - 310.0f);
	moveVal = XMFLOAT3(
		-SinDeg(rotModel.y) * VALUE_MOVE_ENEMY,
		0.0f,
		-CosDeg(rotModel.y) * VALUE_MOVE_ENEMY);
	rotModel = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rotModel;
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);
	// �ۉe�̐���
	shadowNum = CreateShadow(pos, 25.0f);
	use = true;
	isCollision = true;
	bulletTimer = 300;

	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	
	//mapIndex.x = 8;
	//mapIndex.y = 8;


	rootTimer = ROOT_TIME;
	rootIndex = search_Root(mapIndex);

	rootIndexNum = 0;

	myTag = ENEMY;
	collType = Collision::DYNAMIC;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Enemy::Uninit() {
	ReleaseShadow(shadowNum);

	delete rootIndex;

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
void Enemy::Update() {
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;
	XMFLOAT3 ShadowMove = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (!use) {
		return;
	}

	// �����蔻��
	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (hitList[i] == BULLET_PLAYER) {
				// �e�Ɠ���������
				Destroy();
				return;
			}
		}
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
	//pos.x += moveVal.x;
	//pos.y += moveVal.y;
	//pos.z += moveVal.z;
	if (rootIndex->x > -1 && rootIndex->y > -1) {
		pos.x = (*(rootIndex + rootIndexNum)).x * 80.0f - 640.0f;
		pos.z = (*(rootIndex + rootIndexNum)).y * 80.0f + 480.0f;
		rootIndexNum++;
	}

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
	//if (fabsf(rotModel.y - rotDest.y) < 0.0001f) {
	//	if (lr) {
	//		moveVal.x *= -1.0f;
	//	}
	//	if (fb) {
	//		moveVal.z *= -1.0f;
	//	}
	//	if (lr || fb) {
	//		rotDest.y = XMConvertToDegrees(atan2f(-moveVal.x, -moveVal.z));
	//	}
	//}

	//// �ړI�̊p�x�܂ł̍���
	//float fDiffRotY = rotDest.y - rotModel.y;
	//if (fDiffRotY >= 180.0f) {
	//	fDiffRotY -= 360.0f;
	//}
	//if (fDiffRotY < -180.0f) {
	//	fDiffRotY += 360.0f;
	//}

	//// �ړI�̊p�x�܂Ŋ�����������
	//rotModel.y += fDiffRotY * RATE_ROTATE_ENEMY;
	//if (rotModel.y >= 180.0f) {
	//	rotModel.y -= 360.0f;
	//}
	//if (rotModel.y < -180.0f) {
	//	rotModel.y += 360.0f;
	//}


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
		Bullet::FireBullet(
			pos,
			XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLET_ENEMY);

		bulletTimer = BULLET_TIME;
	}

	rootTimer--;
	if (rootTimer < 0) {
	// �}�b�v�̗v�f�ԍ��ł������猻�݂̈ʒu���ǂ��ɂȂ�̂������߂�
		mapIndex.x = (pos.x + 640.0f) / 80.0f;
		mapIndex.y = abs(pos.z - 480.0) / 80.0f;
		rootIndexNum = 0;
		rootIndex = search_Root(mapIndex);
		rootTimer = ROOT_TIME;
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
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

	PrintDebugProc("%f","%f\n", (*(rootIndex + rootIndexNum)).x, (*(rootIndex + rootIndexNum)).y);
}

//====================================================================================
//
//				������
//
//====================================================================================
void Enemy::Destroy() {
	use = false;

	// �ۉe���
	ReleaseShadow(shadowNum);
	shadowNum = -1;
	// �����J�n
	pos.x -= moveVal.x;
	pos.y -= moveVal.y;
	pos.z -= moveVal.z;

	int nExp = StartDwarfEffect(pos);

	isCollision = false;

	GameObjManager::DelList(gameObjNum, false);		// Uninit�����f���Ɗۉe�̊J���݂̂Ȃ̂łȂ��B286�Ŋۉe�̊J���͍s�����A
																// ���f���̊J���̓V���O���g���̈וʂ̓G�`��ɉe�����o�邽�ߍs��Ȃ�
}


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