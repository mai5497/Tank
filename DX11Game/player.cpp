//************************************************************************************
//
// �v���C���[ [player.cpp]
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
#include "GameObject.h"
#include "DebugCollision.h"

//-------------------- �}�N����` --------------------
#define MODEL_PLAYER		"data/model/kobitoblue.fbx"

#define	VALUE_MOVE_PLAYER	(5.0f)	// �ړ����x
#define	RATE_MOVE_PLAYER	(0.25f)	// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��

#define	PLAYER_RADIUS		(10.0f)		// ���E�����a

//-------------------- �O���[�o���ϐ���` --------------------
static CAssimpModel	g_model;		// ���f��

static int			g_nShadow;		// �ۉe�ԍ�

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Player::Player() {

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
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = XMFLOAT3(100.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);




	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}

	// �ۉe�̐���
	g_nShadow = CreateShadow(pos, 12.0f);

}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Player::Uninit() {
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
void Player::Update() {
	moveVal.x = 0.0f;
	moveVal.y = 0.0f;
	moveVal.z = 0.0f;


	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

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

	// �ʒu�ړ�
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.z += moveVal.z;

	// �ړ��ʂɊ�����������
	//g_gameObject.m_move.x += (0.0f - g_gameObject.m_move.x) * RATE_MOVE_PLAYER;
	//g_gameObject.m_move.y += (0.0f - g_gameObject.m_move.y) * RATE_MOVE_PLAYER;
	//g_gameObject.m_move.z += (0.0f - g_gameObject.m_move.z) * RATE_MOVE_PLAYER;


	if (pos.x < -630.0f) {
		pos.x = -630.0f;
	}
	if (pos.x > 630.0f) {
		pos.x = 630.0f;
	}
	if (pos.z < -630.0f) {
		pos.z = -630.0f;
	}
	if (pos.z > 630.0f) {
		pos.z = 630.0f;
	}
	if (pos.y < 10.0f) {
		pos.y = 10.0f;
	}
	if (pos.y > 150.0f) {
		pos.y = 150.0f;
	}

	//if (GetKeyPress(VK_RETURN)) {
	//	// ���Z�b�g
	//	g_posModel = XMFLOAT3(0.0f, 40.0f, 0.0f);
	//	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	mapIndex.x = (pos.x + 640.0f) / 80.0f;
	mapIndex.y = abs(pos.z - 480.0) / 80.0f;

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

	// �ۉe�̈ړ�
	MoveShadow(g_nShadow, pos);

	if ((moveVal.x * moveVal.x
		+ moveVal.y * moveVal.y
		+ moveVal.z * moveVal.z) > 1.0f) {
		XMFLOAT3 pos;

		pos.x = pos.x + SinDeg(rotModel.y) * 10.0f;
		pos.y = pos.y + 2.0f;
		pos.z = pos.z + CosDeg(rotModel.y) * 10.0f;

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
		FireBullet(pos, XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
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
void Player::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	g_model.Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����


	//PrintDebugProc("%d,%d\n", g_gameObject.m_mapIndex.x, g_gameObject.m_mapIndex.y);
	//PrintDebugProc("%f\n", g_gameObject.m_pos.z);

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

//====================================================================================
//
//				�Փ˔���
//
//====================================================================================
bool Player::CollisionPlayer(XMFLOAT3 pos, float radius, float damage) {
	bool hit = CollisionSphere(pos, PLAYER_RADIUS, pos, radius);
	if (hit) {
		// �����J�n
		int nExp = -1;
		if (damage > 0.0f) {
			nExp = StartExplosion(pos, XMFLOAT2(40.0f, 40.0f));
			// 
		} else {
			nExp = StartExplosion(pos, XMFLOAT2(20.0f, 20.0f));
		}
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
	}
	return hit;
}

bool Player::CollisionPlayer(GameObject collision) {
	GameObject* _GameObject;

	_GameObject = this;

	bool isHit = CollisionSphere(*_GameObject, collision);

	if (isHit) {
		_GameObject = Push(pos, XMFLOAT3(PLAYER_RADIUS, PLAYER_RADIUS, PLAYER_RADIUS), moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));

		if (_GameObject == nullptr) {
			//�@�����o���Ɏ��s���Ă���
			return isHit;
		}

		pos = _GameObject->pos;
		moveVal = _GameObject->moveVal;
	}

	return isHit;
}

XMINT2 Player::SetPlayerIndex() {
	return mapIndex;
}