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
#include "BulletLine.h"
#include "Game.h"
#include "Texture.h"

//-------------------- �萔��` --------------------
#define MODEL_ENEMY			("data/model/kobitored.fbx")
#define TOON_TEXTURE		("data/model/ramp.png")

#define	VALUE_MOVE_ENEMY	(0.50f)		// �ړ����x
#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��
#define ENEMY_RADIUS		(50.0f)		// ���E�����a

#define BULLET_TIME			(120)		// �e���˂܂ł̎���
#define ROOT_TIME			(300)		// ���Ƀ��[�g��������܂ł̎���
#define MOVE_TIME			(30)		// ���Ƀ��[�g��������܂ł̎���


std::unique_ptr<CAssimpModel> Enemy::pMyModel;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Enemy::Enemy(int mapIndex_x, int mapindex_y,Game *_pGameScene) {
	// �C���f�b�N�X��ۑ�
	mapIndex.x = mapIndex_x;
	mapIndex.y = mapindex_y;

	// �Q�[���V�[����ۑ�
	pGameScene = _pGameScene;
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
	//----- ���f���f�[�^�̓ǂݍ��� -----
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

		// �g�D�[���V�F�[�_�[
		std::unique_ptr<Texture> _pTexture = std::make_unique<Texture>();
		_pTexture->SetTexture(pDevice, TOON_TEXTURE);
		pMyModel->SetShaderMode(CAssimpModel::SM_TOON);	// �g�D�[���V�F�[�_�[�ɂ���
		pMyModel->SetShaderMat(_pTexture->GetTexture());


		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		}
	}

	//----- �ʒu�E��]�E�X�P�[���E�T�C�Y�ȂǃI�u�W�F�N�g�ɕK�v�ȏ����ݒ� -----
	pos = XMFLOAT3(mapIndex.x * 80.0f - 640.0f, 50.0f, -mapIndex.y * 80.0f + 480.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f,0.0f);
	rotModel = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
	rotDest = rotModel;
	size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);
	myTag = ENEMY;								// �I�u�W�F�N�g���ʂ̃^�O
	collType = Collision::DYNAMIC;				// ���I�I�u�W�F�N�g

	//----- �G�̏����ݒ� -----
	shadowNum = CreateShadow(pos, 25.0f);	// �ۉe�̐���

	use = true;								// �g�p�t���O
	
	isCollision = true;						// �����蔻����s����
	
	bulletTimer = 300;						// �e���˂̊Ԋu

	mapIndex.x = (pos.x + 640.0f) / 80.0f;	// ���W���}�b�v�ԍ��ɕϊ������l
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	rootTimer = 0;							// ���[�g�����̊Ԋu
	rootIndex = search_Root(mapIndex);		// ���[�g���������ē����
	rootIndexNum = rootIndex.end()-1;		// ���[�g�̌������ʂ���납�����̂Ō��������l�Ƃ���

	//----- �e�̗\�����̏����� -----
	pBulletLine = std::make_unique<BulletLine>();
	pBulletLine->Init(this);

	//----- �v���C���[�̈ʒu�擾 -----
	playerPos = pGameScene->GetPlayerPos();
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Enemy::Uninit() {
	// �e�̗\�����̊J��
	pBulletLine->Uninit();
	pBulletLine.reset();

	// �e�̉��
	ReleaseShadow(shadowNum);

	// �T���̃��[�g��vector�̏I��
	rootIndex.clear();

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
	// ���g�p�Ȃ�X�L�b�v
	if (!use) {
		return;
	}

	//----- �ϐ������� -----
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;

	//----- �����蔻�� -----
	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (hitList[i].myTag == BULLET_PLAYER) {
				// �e�Ɠ���������
				Destroy();
				return;
			}
		}
	}

	//----- �ړ� -----
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	// ���W���猻�݂̃}�b�v�ԍ����擾
	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ���������[�g�ƌ��݂̈ʒu���r���Ĉړ��ʂ����߂�
	if ((*rootIndexNum).x != -1 && abs(pos.x - ((*rootIndexNum).x * 80 - 640 + 40)) > 1.0f) {
		if (pos.x - ((*rootIndexNum).x * 80 - 640 + 40) < -1.0f) {
			moveVal.x = VALUE_MOVE_ENEMY;
			rotDest.y = rotCamera.y - 90.0f;
		} else if (pos.x - ((*rootIndexNum).x * 80 - 640 + 40) > 1.0f) {
			moveVal.x = -VALUE_MOVE_ENEMY;
			rotDest.y = rotCamera.y + 90.0f;
		}
	} else if ((*rootIndexNum).y != -1 && abs(pos.y - ((-(*rootIndexNum).y * 80.0f) - 480.0f)) > 1.0f) {
		if (pos.y - ((-(*rootIndexNum).y * 80.0f) - 480.0f) < -1.0f) {
			moveVal.z = -VALUE_MOVE_ENEMY;
			rotDest.y = rotCamera.y;
		} else if (pos.y - ((-(*rootIndexNum).y * 80.0f) - 480.0f) > 1.0f) {
			moveVal.z = VALUE_MOVE_ENEMY;
			rotDest.y = rotCamera.y + 180.0f;
		}
	} else {
		moveVal.z = 0.0f;
	}

	// ���̃��[�g�ɍX�V
	if (mapIndex.x == (*rootIndexNum).x && mapIndex.y == (*rootIndexNum).y){
		if (rootIndex.size() > 1 && (rootIndexNum->x > -1 && rootIndexNum->y > -1)) {
			rootIndexNum--;
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

	// �ړ��ʂ̉��Z
	pos.x += moveVal.x;
	pos.z += moveVal.z;


	//----- �ړ������[���h���W�ɕϊ����� -----
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

	// �e�̈ړ�
	MoveShadow(shadowNum, pos);

	//----- �e�̗\�����̍X�V -----
	XMFLOAT3 dir;

	// �v���C���[�̈ʒu�擾
	playerPos = pGameScene->GetPlayerPos();
	int distanceRate = pGameScene->GetStageNum()+1 / MAX_STAGE-1;

	if (abs(playerPos.x - pos.x + playerPos.z - pos.z) < 500.0f * distanceRate) {
		dir = XMFLOAT3(playerPos.x - pos.x, 0.0f, playerPos.z - pos.z);
	} else {
		dir = XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33);
	}
	pBulletLine->SetDir(dir);
	pBulletLine->Update();

	//----- �e���� -----
	int randomtime;
	randomtime = rand() % 3;
	bulletTimer -= randomtime;
	if (bulletTimer < 0) {
		if (pGameScene->GetStageNum()+1 > MAX_STAGE / 2) {
			Bullet::FireBullet(pos, XMFLOAT3(dir), BULLET_ENEMY, gameObjNum, BULLETTYPE_MISSILE);
		} else {
			Bullet::FireBullet(pos, XMFLOAT3(dir), BULLET_ENEMY, gameObjNum, BULLETTYPE_NORMAL);
		}
		//float angle = 45 * 3.14 / 180;

		//afterDir.x = dir.x * cos(angle) - dir.z * sin(angle);
		//afterDir.z= dir.x * sin(angle) + dir.z * cos(angle);


		//Bullet::FireBullet(pos,XMFLOAT3(bulletWorld._31, bulletWorld._32, bulletWorld._33),BULLET_ENEMY,gameObjNum);
		//Bullet::FireBullet(pos,XMFLOAT3(-afterDir.x,0,afterDir.z),BULLET_ENEMY,gameObjNum);

		bulletTimer = BULLET_TIME;
	}

	//----- ���[�g���� -----
	rootTimer--;
	if (rootTimer < 0) {
		// �}�b�v�̗v�f�ԍ��ł������猻�݂̈ʒu���ǂ��ɂȂ�̂������߂�
		mapIndex.x = (pos.x + 640.0f) / 80.0f;
		mapIndex.y = abs(pos.z - 480.0) / 80.0f;
		rootIndex = search_Root(mapIndex);
		rootIndexNum = rootIndex.end() - 1;
		rootTimer = ROOT_TIME;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Enemy::Draw() {
	// ���g�p�Ȃ�X�L�b�v
	if (!use) {
		return;
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

	// �e�̗\�����̕`��
	pBulletLine->Draw();

	//PrintDebugProc("moveval:%f,%f\n", moveVal.x, moveVal.y);
	//PrintDebugProc("mapIndex:%d,%d\n", mapIndex.x, mapIndex.y);
	//PrintDebugProc("rootIndex:%d,%d\n", (*rootIndexNum).x, (*rootIndexNum).y);
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

	GameObjManager::DelList(gameObjNum, false);		// Uninit�����f���Ɗۉe�̊J���݂̂Ȃ̂łȂ��B���傢��Ŋۉe�̊J���͍s�����A
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