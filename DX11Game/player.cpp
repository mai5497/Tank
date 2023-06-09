//************************************************************************************
//
// �v���C���[ [player.cpp]
// Author : �ɒn�c�^��
//
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Player.h"

#include "input.h"
#include "debugproc.h"
#include "DebugCollision.h"

#include "Texture.h"

#include "fade.h"
#include "Sound.h"

#include "shadow.h"
#include "explosion.h"
#include "effect.h"

#include "Game.h"
#include "Bullet.h"
#include "Astar.h"
#include "BulletLine.h"

//-------------------- �}�N����` --------------------
#define MODEL_PLAYER		("data/model/kobitoblue.fbx")
#define TOON_TEXTURE		("data/model/ramp.png")

#define	VALUE_MOVE_PLAYER	(1.0f)		// �ړ����x
#define	RATE_MOVE_PLAYER	(0.25f)		// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��

#define	PLAYER_RADIUS		(10.0f)		// ���E�����a

std::unique_ptr<CAssimpModel> Player::pMyModel;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Player::Player(Game* _pGameScene) {
	pGameScene = _pGameScene;
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Player::~Player() {
}


//====================================================================================
//
//				������
//
//====================================================================================
void Player::Init() {

	//----- �ʒu�E��]�E�X�P�[���ȂǃI�u�W�F�N�g�ɕK�v�ȏ����ݒ� -----
	pos = XMFLOAT3(-300.0f, 50.0f, 350.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);

	isCollision = true;

	myTag = PLAYER;
	collType = Collision::DYNAMIC;

	// ���f���f�[�^�̓ǂݍ���
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		// �g�D�[���V�F�[�_�[
		std::unique_ptr<Texture> _pTexture = std::make_unique<Texture>();
		_pTexture->SetTexture(pDevice, TOON_TEXTURE);
		pMyModel->SetShaderMode(CAssimpModel::SM_TOON);	// �g�D�[���V�F�[�_�[�ɂ���
		pMyModel->SetShaderMat(_pTexture->GetTexture());

		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
			MessageBoxA(GetMainWnd(), "�v���C���[���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		}
	}

	// �ۉe�̐���
	shadowNum = CreateShadow(pos, 20.0f);

	// HP�̏�����
	hitPoint = MAX_HP;
	// �Q�[���V�[����HP��ۑ�����
	pGameScene->StoragePlayerHP(hitPoint);	

	// �e�̗\�����̏�����
	pBulletLine = std::make_unique<BulletLine>();
	pBulletLine->Init(this);

	// ���W���Q�[���V�[���ɕۑ�
	pGameScene->StoragePlayerPos(pos);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Player::Uninit() {
	// �e�̗\�����̊J��
	pBulletLine->Uninit();
	pBulletLine.reset();

	// �ۉe�̉��
	ReleaseShadow(shadowNum);

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
void Player::Update() {
	//----- �J�����̌����擾 -----
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	//----- �ړ��̓��͏��� -----
	if (GetKeyPress(VK_A)) {
		if (GetKeyPress(VK_W)) {
			// ���O�ړ�
			moveVal.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// ����ړ�
			moveVal.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// ���ړ�
			moveVal.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_D)) {
		if (GetKeyPress(VK_W)) {
			// �E�O�ړ�
			moveVal.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_S)) {
			// �E��ړ�
			moveVal.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// �E�ړ�
			moveVal.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			moveVal.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;

			rotDestModel.y = rotCamera.y - 90.0f;
		}
	} else if (GetKeyPress(VK_W)) {
		// �O�ړ�
		moveVal.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
		moveVal.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;

		rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_S)) {
		// ��ړ�
		moveVal.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		moveVal.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;

		rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_J)) {
		// ����]
		rotDestModel.y -= VALUE_ROTATE_PLAYER;
		if (rotDestModel.y < -180.0f) {
			rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_L)) {
		// �E��]
		rotDestModel.y += VALUE_ROTATE_PLAYER;
		if (rotDestModel.y >= 180.0f) {
			rotDestModel.y -= 360.0f;
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = rotDestModel.y - rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (rotModel.y >= 180.0f) {
		rotModel.y -= 360.0f;
	}
	if (rotModel.y < -180.0f) {
		rotModel.y += 360.0f;
	}

	// �ړ��ʂɊ�����������
	moveVal.x += (0.0f - moveVal.x) * RATE_MOVE_PLAYER;
	moveVal.y += (0.0f - moveVal.y) * RATE_MOVE_PLAYER;
	moveVal.z += (0.0f - moveVal.z) * RATE_MOVE_PLAYER;

	// �ʒu�ړ�
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	//----- �ړ������[���h���W�ɕϊ� -----
	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate, _mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	_mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	_mtxScale = XMMatrixScaling(size.x, size.y, size.z);
	_mtxWorld = XMMatrixMultiply(_mtxScale, _mtxWorld);

	// ��]�𔽉f
	_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotModel.x),
		XMConvertToRadians(rotModel.y), XMConvertToRadians(rotModel.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// �ړ��𔽉f
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&mtxWorld, _mtxWorld);


	//----- �ۉe�̈ړ� -----
	MoveShadow(shadowNum, pos);

	//----- �ړ����̃G�t�F�N�g -----
	if ((moveVal.x * moveVal.x
		+ moveVal.y * moveVal.y
		+ moveVal.z * moveVal.z) > 1.0f) {
		XMFLOAT3 effPos;

		effPos.x = pos.x + SinDeg(rotModel.y) * 10.0f;
		effPos.y = pos.y + 2.0f;
		effPos.z = pos.z + CosDeg(rotModel.y) * 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.85f, 0.05f, 0.65f, 0.50f),
			XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f),
			XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(effPos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
			XMFLOAT2(5.0f, 5.0f), 20);
	}

	//----- �e -----
	// �e����
	XMFLOAT3 dir = XMFLOAT3(GetMousePosition()->x-SCREEN_WIDTH / 2 - pos.x, 0.0f, -(GetMousePosition()->y - SCREEN_HEIGHT / 2) - pos.z);
	if (GetKeyRelease(VK_SPACE) || GetMouseRelease(MOUSEBUTTON_L)) {
		Bullet::FireBullet(pos, XMFLOAT3(dir.x, dir.y, dir.z),
			BULLET_PLAYER,
			gameObjNum);


	}

	// �e�̗\�����̍X�V
	pBulletLine->SetDir(dir);
	pBulletLine->Update();

	//----- ���݂̍��W���}�b�v�̔z��̗v�f�ԍ��ɂ����� -----
	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

	//----- A*�Ɏ����̍��W��n�� -----
	SetPlayerIndex(mapIndex);

	//----- ���W���Q�[���V�[���ɕۑ� -----
	pGameScene->StoragePlayerPos(pos);

	//----- �����蔻�� -----
	if (hitList.size() > 0) {
		for (int i = 0; i < hitList.size(); i++) {
			if (hitList[i].myTag == BULLET_ENEMY) {
				// �e�Ɠ���������
				hitPoint--;	//HP�����炷
		
				pGameScene->StoragePlayerHP(hitPoint);	// �Q�[���V�[����HP��ۑ�����
				
				if (hitPoint > 0) {
					CSound::Play(SE_DAMAGE);
				} else {
					CSound::Play(SE_KILL);
				}
			}
		}
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Player::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	pMyModel->Draw(pDC, mtxWorld, eOpacityOnly);

	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

	// �e�̗\�����̕`��
	pBulletLine->Draw();

	//PrintDebugProc("index:%d,%d\n",mapIndex.x, mapIndex.y);
	//PrintDebugProc("pos:%f,%f,%f\n",pos.x, pos.y,pos.z);
	//PrintDebugProc("pos:%f,%f,%f\n",testPos.x, testPos.y, testPos.z);
	//PrintDebugProc("%f\n", mtxWorld._32);
	//PrintDebugProc("%f\n", moveVal.y);
	//PrintDebugProc("%f\n", moveVal.z);

	//DrawCollisionSphere(g_gameObject);

}

//====================================================================================
//
//				�ʒu�擾
//
//====================================================================================
XMFLOAT3& Player::GetPlayerPos() {
	return pos;
}
