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
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "DebugCollision.h"
#include "Texture.h"

//-------------------- �}�N����` --------------------
#define MODEL_PLAYER	"data/model/kobitoblue.fbx"
#define TOON_TEXTURE	"data/model/ramp.png"

#define	VALUE_MOVE_PLAYER	(0.5f)	// �ړ����x
#define	RATE_MOVE_PLAYER	(0.25f)	// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��

#define	PLAYER_RADIUS		(10.0f)		// ���E�����a

std::unique_ptr<CAssimpModel> Player::pMyModel;

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
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = XMFLOAT3(100.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	size = XMFLOAT3(50.0f, 50.0f, 50.0f);
	collSize = XMFLOAT3(50.0f, 50.0f, 50.0f);

	isCollision = true;

	// ���f���f�[�^�̓ǂݍ���
	if (!pMyModel) {
		pMyModel = std::make_unique<CAssimpModel>();
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


		// �g�D�[���V�F�[�_�[
		ID3D11ShaderResourceView* _pTexture = nullptr;		// �e�N�X�`��
		CreateTextureFromFile(pDevice, TOON_TEXTURE, &_pTexture);
		pMyModel->SetShaderMode(CAssimpModel::SM_TOON);	// �g�D�[���V�F�[�_�[�ɂ���
		pMyModel->SetShaderMat(_pTexture);

		if (!pMyModel->Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
			MessageBoxA(GetMainWnd(), "�v���C���[���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		}
	}
	// �ۉe�̐���
	shadowNum = CreateShadow(pos, 12.0f);

	myTag = PLAYER;
	collType = Collision::DYNAMIC;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Player::Uninit() {
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
	moveVal.x += (0.0f - moveVal.x) * RATE_MOVE_PLAYER;
	moveVal.y += (0.0f - moveVal.y) * RATE_MOVE_PLAYER;
	moveVal.z += (0.0f - moveVal.z) * RATE_MOVE_PLAYER;


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
	MoveShadow(shadowNum, pos);

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

	//static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	//double dNowTime = clock() / double(CLOCKS_PER_SEC);
	//double dSlice = dNowTime - dLastTime;
	//dLastTime = dNowTime;
	//m_animTime += dSlice;
	//if (m_animTime >= myModel.GetAnimDuration(0)) {
	//	m_animTime = 0.0;
	//}


	// �e����
	if (GetKeyRepeat(VK_SPACE)) {
		Bullet::FireBullet(pos, XMFLOAT3(-mtxWorld._31, -mtxWorld._32, -mtxWorld._33),
			BULLET_PLAYER);
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

	// ������������`��
	//myModel.SetAnimIndex(1);
	//myModel.SetAnimTime(m_animTime);
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	pMyModel->Draw(pDC, mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����


	//PrintDebugProc("%d,%d\n", g_gameObject.m_mapIndex.x, g_gameObject.m_mapIndex.y);
	//PrintDebugProc("%f\n", moveVal.x);
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

//====================================================================================
//
//				�Փ˔���
//
//====================================================================================
bool Player::CollisionPlayer(XMFLOAT3 pos, float radius, float damage) {
	Collision _collision;
	bool hit = _collision.CollisionSphere(pos, PLAYER_RADIUS, pos, radius);
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
	Collision _collision;

	_GameObject = this;

	bool isHit = _collision.CollisionSphere(*_GameObject, collision);

	if (isHit) {
		_GameObject = _collision.Push(pos, XMFLOAT3(PLAYER_RADIUS, PLAYER_RADIUS, PLAYER_RADIUS), moveVal, collision.pos, XMFLOAT3(collision.collRadius, collision.collRadius, collision.collRadius));

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