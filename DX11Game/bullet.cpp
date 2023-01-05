//************************************************************************************
// 
// �e[bullet.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "bullet.h"
#include "Camera.h"
#include "shadow.h"
#include "Texture.h"
#include "Light.h"
#include "explosion.h"
#include "smoke.h"
#include "effect.h"
#include "debugproc.h"
#include "GameObjManager.h"

//-------------------- �萔��` --------------------
#define	PLAYERBULLET_FILENAME	L"data/texture/bullet_p.png"	// �e�N�X�`�� �t�@�C����
#define	ENEMYBULLET_FILENAME	L"data/texture/bullet_e.png"	// �e�N�X�`�� �t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BULLET			100

#define BULLET_SPEED		7.5f
#define EBULLET_SPEED		5.5f
#define BULLET_RADIUS		10.0f
#define BULLET_STRENGTH		1.0f
#define BULLET_GRAVITY		0.95f

#define MIN_FIELD_X			-640.0f
#define MAX_FIELD_X			640.0f

#define MIN_FIELD_Y			(10.0f)

#define MIN_FIELD_Z			-640.0f
#define MAX_FIELD_Z			640.0f

MESH Bullet::mesh_p;					// ���b�V�����
MESH Bullet::mesh_e;					// ���b�V�����
MATERIAL Bullet::material;				// �}�e���A��


//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Bullet::Bullet() {

}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Bullet::~Bullet() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void Bullet::Init(void) {
	ID3D11Device* pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �}�e���A���̐ݒ�
	material.Diffuse = M_DIFFUSE;
	material.Ambient = M_AMBIENT;
	material.Specular = M_SPECULAR;
	material.Power = M_POWER;
	material.Emissive = M_EMISSIVE;
	mesh_p.pMaterial = &material;
	mesh_e.pMaterial = &material;

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice, PLAYERBULLET_FILENAME, &mesh_p.pTexture);
	CreateTextureFromFile(pDevice, ENEMYBULLET_FILENAME, &mesh_e.pTexture);

	XMStoreFloat4x4(&mesh_p.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_e.mtxTexture, XMMatrixIdentity());

	// ���[���h �}�g���b�N�X������
	XMStoreFloat4x4(&mesh_p.mtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&mesh_e.mtxWorld, XMMatrixIdentity());

	// �e��񏉊���
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	moveVal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	collSize = XMFLOAT3(25.0f, 25.0f, 25.0f);
	use = false;
	isCollision = false;
	shadowNum = -1;
	collType = Collision::DYNAMIC;
	type = BULLETTYPE_PLAYER;
	myTag = BULLET_PLAYER;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Bullet::Uninit() {
	if (use) {
		use = false;
		ReleaseShadow(shadowNum);
		shadowNum = -1;
	}
	ReleaseMesh(&mesh_p);
	ReleaseMesh(&mesh_e);
}


//====================================================================================
//
//				�X�V
//
//====================================================================================
void Bullet::Update() {
	//for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		// ���g�p�Ȃ�X�L�b�v
	if (!use) {
		return;
	}
	// �ʒu���X�V
	pos.x += moveVal.x;
	pos.y += moveVal.y;
	pos.y *= BULLET_GRAVITY;
	pos.z += moveVal.z;
	// �͈̓`�F�b�N
	if (pos.x < MIN_FIELD_X || pos.x > MAX_FIELD_X ||
		//pBullet->pos.y < MIN_FIELD_Y ||
		pos.z < MIN_FIELD_Z || pos.z > MAX_FIELD_Z) {

		Destroy();

		return;
	}
	if (type == BULLETTYPE_PLAYER) {
		if (hitList.size() > 0) {
			for (int i = 0; i < hitList.size(); i++) {
				if (hitList[i] == ENEMY) {	// �v���C���[�Ƃ̓����蔻��
					Destroy();
				} else if (hitList[i] == WALL) {	// �ǂƂ̓����蔻��
					Destroy();
				} else if (hitList[i] == BULLET_ENEMY) {	// �e�Ƃ̓����蔻��
					Destroy();
				}
			}
		}
	} else if (type == BULLETTYPE_ENEMY) {
		if (hitList.size() > 0) {
			for (int i = 0; i < hitList.size(); i++) {
				if (hitList[i] == PLAYER) {	// �v���C���[�Ƃ̓����蔻��
					Destroy();
				}else if(hitList[i] == ENEMY) {	// �����ȊO�̓G�Ƃ̓����蔻��
					Destroy();
				}else if (hitList[i] == WALL) {	// �ǂƂ̓����蔻��
					Destroy();
				} else if (hitList[i] == BULLET_PLAYER) {	// �e�Ƃ̓����蔻��
					Destroy();
				}
			}
		}


	}

	// �ۉe�ړ�
	MoveShadow(shadowNum, pos);
	// �G�t�F�N�g�̐ݒ�
	if (type == BULLETTYPE_PLAYER) {
		// ������
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f));
	} else {
		// ������
		SetSmoke(pos, XMFLOAT2(8.0f, 8.0f));

	}

}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Bullet::Draw() {
	// ���g�p�Ȃ�X�L�b�v
	if (!use) {
		return;
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// ��������
	SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();

	if (type == BULLETTYPE_PLAYER) {
		// �r���[�s��̉�]�����̓]�u�s���ݒ�
		mesh_p.mtxWorld._11 = mView._11;
		mesh_p.mtxWorld._12 = mView._21;
		mesh_p.mtxWorld._13 = mView._31;
		mesh_p.mtxWorld._14 = 0.0f;
		mesh_p.mtxWorld._21 = mView._12;
		mesh_p.mtxWorld._22 = mView._22;
		mesh_p.mtxWorld._23 = mView._32;
		mesh_p.mtxWorld._24 = 0.0f;
		mesh_p.mtxWorld._31 = mView._13;
		mesh_p.mtxWorld._32 = mView._23;
		mesh_p.mtxWorld._33 = mView._33;
		mesh_p.mtxWorld._34 = 0.0f;
		// �ʒu�𔽉f
		mesh_p.mtxWorld._41 = pos.x;
		mesh_p.mtxWorld._42 = pos.y;
		mesh_p.mtxWorld._43 = pos.z;
		mesh_p.mtxWorld._44 = 1.0f;
		// �`��
		DrawMesh(pDC, &mesh_p);

	} else {
		// �r���[�s��̉�]�����̓]�u�s���ݒ�
		mesh_e.mtxWorld._11 = mView._11;
		mesh_e.mtxWorld._12 = mView._21;
		mesh_e.mtxWorld._13 = mView._31;
		mesh_e.mtxWorld._14 = 0.0f;
		mesh_e.mtxWorld._21 = mView._12;
		mesh_e.mtxWorld._22 = mView._22;
		mesh_e.mtxWorld._23 = mView._32;
		mesh_e.mtxWorld._24 = 0.0f;
		mesh_e.mtxWorld._31 = mView._13;
		mesh_e.mtxWorld._32 = mView._23;
		mesh_e.mtxWorld._33 = mView._33;
		mesh_e.mtxWorld._34 = 0.0f;
		// �ʒu�𔽉f
		mesh_e.mtxWorld._41 = pos.x;
		mesh_e.mtxWorld._42 = pos.y;
		mesh_e.mtxWorld._43 = pos.z;
		mesh_e.mtxWorld._44 = 1.0f;
		// �`��
		DrawMesh(pDC, &mesh_e);
	}


	SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//====================================================================================
//
//				����
//
//====================================================================================
void Bullet::FireBullet(XMFLOAT3 _pos, XMFLOAT3 _dir, EBulletType _type) {
	std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>();

	pBullet->Init();

	pBullet->pos = _pos;
	// �����x�N�g���𐳋K��
	XMStoreFloat3(&_dir, XMVector3Normalize(XMLoadFloat3(&_dir)));
	if (_type == BULLETTYPE_PLAYER) {
		pBullet->moveVal.x = _dir.x * BULLET_SPEED;
		pBullet->moveVal.y = _dir.y * BULLET_SPEED;
		pBullet->moveVal.z = _dir.z * BULLET_SPEED;
	} else {
		pBullet->moveVal.x = _dir.x * EBULLET_SPEED;
		pBullet->moveVal.y = _dir.y * EBULLET_SPEED;
		pBullet->moveVal.z = _dir.z * EBULLET_SPEED;
	}
	pBullet->shadowNum = CreateShadow(_pos, BULLET_RADIUS);
	XMFLOAT4X4 _mtxWorld;
	XMStoreFloat4x4(&_mtxWorld,
		XMMatrixTranslation(pBullet->pos.x, pBullet->pos.y, pBullet->pos.z));
	pBullet->use = true;
	pBullet->type = _type;
	pBullet->isCollision = true;

	pBullet->gameObjNum = GameObjManager::AddList(pBullet, false);	// ��ł��łɏ������͂��Ă���̂ŏ������͕K�v�Ȃ�

	pBullet.reset();
}

//====================================================================================
//
//				���_���̍쐬
//
//====================================================================================
void Bullet::MakeVertexBullet(ID3D11Device* pDevice) {
	// �ꎞ�I�Ȓ��_�z��𐶐�
	mesh_p.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[mesh_p.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-BULLET_RADIUS, BULLET_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(BULLET_RADIUS, BULLET_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	mesh_p.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	MakeMeshVertex(pDevice, &mesh_p, pVertexWk, pIndexWk);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
}


//====================================================================================
//
//				������
//
//====================================================================================
void Bullet::Destroy() {
	use = false;
	// �ۉe���
	ReleaseShadow(shadowNum);
	shadowNum = -1;
	// �����J�n
	pos.x -= moveVal.x;
	pos.y -= moveVal.y;
	pos.z -= moveVal.z;
	int nExp = StartExplosion(pos, XMFLOAT2(20.0f, 20.0f));

	isCollision = false;

	GameObjManager::DelList(gameObjNum);
}

