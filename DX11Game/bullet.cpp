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
#include "mesh.h"
#include "Light.h"
#include "explosion.h"
#include "smoke.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "debugproc.h"
#include "Boss.h"
#include "WallObject.h"

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

//-------------------- �\���̒�` --------------------
struct TBullet {
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT3	vel;		// ���x
	int			nShadow;	// �eID
	bool		use;		// �g�p���Ă��邩�ǂ���
	EBulletType	type;		// ���
};

//-------------------- �O���[�o���ϐ���` --------------------
static MESH			g_mesh_p;					// ���b�V�����
static MESH			g_mesh_e;					// ���b�V�����
static MATERIAL		g_material;				// �}�e���A��
static TBullet		g_bullet[MAX_BULLET];	// �e���

//-------------------- �v���g�^�C�v�錾 --------------------
static HRESULT MakeVertexBullet(ID3D11Device* pDevice);

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT InitBullet(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// ���_���̍쐬
	hr = MakeVertexBullet(pDevice);
	if (FAILED(hr)) {
		return hr;
	}

	// �}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh_p.pMaterial = &g_material;
	g_mesh_e.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, PLAYERBULLET_FILENAME, &g_mesh_p.pTexture);
	if (FAILED(hr)) {
		return hr;
	}	hr = CreateTextureFromFile(pDevice, ENEMYBULLET_FILENAME, &g_mesh_e.pTexture);
	if (FAILED(hr)) {
		return hr;
	}

	XMStoreFloat4x4(&g_mesh_p.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mesh_e.mtxTexture, XMMatrixIdentity());

	// ���[���h �}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh_p.mtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mesh_e.mtxWorld, XMMatrixIdentity());

	// �e��񏉊���
	for (int i = 0; i < MAX_BULLET; ++i) {
		g_bullet[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bullet[i].vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_bullet[i].use = false;
		g_bullet[i].nShadow = -1;
		g_bullet[i].type = BULLETTYPE_PLAYER;
	}

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitBullet(void)
{
	TBullet* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (pBullet->use) {
			pBullet->use = false;
			ReleaseShadow(pBullet->nShadow);
			pBullet->nShadow = -1;
		}
	}
	ReleaseMesh(&g_mesh_p);
	ReleaseMesh(&g_mesh_e);
}


//====================================================================================
//
//				�X�V
//
//====================================================================================
void UpdateBullet(void)
{
	TBullet* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		// ���g�p�Ȃ�X�L�b�v
		if (!pBullet->use) {
			continue;
		}
		// �ʒu���X�V
		pBullet->pos.x += pBullet->vel.x;
		pBullet->pos.y += pBullet->vel.y;
		pBullet->pos.y *= BULLET_GRAVITY;
		pBullet->pos.z += pBullet->vel.z;
		// �͈̓`�F�b�N
		if (pBullet->pos.x < MIN_FIELD_X || pBullet->pos.x > MAX_FIELD_X ||
			//pBullet->pos.y < MIN_FIELD_Y ||
			pBullet->pos.z < MIN_FIELD_Z || pBullet->pos.z > MAX_FIELD_Z) {
			pBullet->use = false;
			// �ۉe���
			ReleaseShadow(pBullet->nShadow);
			pBullet->nShadow = -1;
			// �����J�n
			pBullet->pos.x -= pBullet->vel.x;
			pBullet->pos.y -= pBullet->vel.y;
			pBullet->pos.z -= pBullet->vel.z;
			int nExp = StartExplosion(pBullet->pos, XMFLOAT2(20.0f, 20.0f));
			//if (pBullet->type == BULLETTYPE_ENEMY) {
			//	SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
			//}
			// ���U�G�t�F�N�g�J�n
			//g_num = SetEffect(pBullet->pos,	XMFLOAT4(0.85f, 0.05f, 0.25f, 0.80f), XMFLOAT2(8.0f, 8.0f), 50);
			
			continue;
		}
		if (pBullet->type == BULLETTYPE_PLAYER) {
			// �G�Ƃ̓����蔻��
			int nEnemy = CollisionEnemy(pBullet->pos, BULLET_RADIUS, BULLET_STRENGTH);
			if (nEnemy > 0) {
				pBullet->use = false;
				// �ۉe���
				ReleaseShadow(pBullet->nShadow);
				pBullet->nShadow = -1;
				continue;
			}
			// �{�X�Ƃ̓����蔻��
			if (GetEnemyKillSum() == MAX_ENEMY) {
				int nBoss = CollisionBoss(pBullet->pos, BULLET_RADIUS, BULLET_STRENGTH);
				if (nBoss > 0) {
					pBullet->use = false;
					// �ۉe�J��
					ReleaseShadow(pBullet->nShadow);
					pBullet->nShadow = -1;
					continue;
				}
			}
			if (CollisionWalltoBullet(pBullet->pos, BULLET_RADIUS, BULLET_STRENGTH)) {
				pBullet->use = false;
				// �ۉe���
				ReleaseShadow(pBullet->nShadow);
				pBullet->nShadow = -1;
				continue;
			}

		} else if (pBullet->type == BULLETTYPE_ENEMY) {
			// �v���C���[�Ƃ̓����蔻��



			//if (CollisionPlayer(pBullet->pos, BULLET_RADIUS, BULLET_STRENGTH)) {
			//	pBullet->use = false;
			//	// �ۉe���
			//	ReleaseShadow(pBullet->nShadow);
			//	pBullet->nShadow = -1;
			//	continue;
			//}




			if (CollisionWalltoBullet(pBullet->pos, BULLET_RADIUS, BULLET_STRENGTH)) {
				pBullet->use = false;
				// �ۉe���
				ReleaseShadow(pBullet->nShadow);
				pBullet->nShadow = -1;
				continue;
			}
		}
		if (!pBullet->use) {
			continue;
		}

		// �ۉe�ړ�
		MoveShadow(pBullet->nShadow, pBullet->pos);
		// �G�t�F�N�g�̐ݒ�
		if (pBullet->type == BULLETTYPE_PLAYER) {
			// ������
			SetSmoke(pBullet->pos, XMFLOAT2(8.0f, 8.0f));
		} else {
			//SetEffect(pBullet->pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			//	XMFLOAT4(0.85f, 0.05f, 0.25f, 0.50f), XMFLOAT2(16.0f, 16.0f), 30);
			//SetEffect(pBullet->pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			//	XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f), XMFLOAT2(12.0f, 12.0f), 30);
			//SetEffect(pBullet->pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
			//	XMFLOAT4(0.45f, 0.45f, 0.05f, 0.20f), XMFLOAT2(6.0f, 6.0f), 30);
			
			// ������
			SetSmoke(pBullet->pos, XMFLOAT2(8.0f, 8.0f));

		}
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawBullet(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// ��������
	SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TBullet* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		// ���g�p�Ȃ�X�L�b�v
		if (!pBullet->use) {
			continue;
		}
		if (pBullet->type == BULLETTYPE_PLAYER) {
			// �r���[�s��̉�]�����̓]�u�s���ݒ�
			g_mesh_p.mtxWorld._11 = mView._11;
			g_mesh_p.mtxWorld._12 = mView._21;
			g_mesh_p.mtxWorld._13 = mView._31;
			g_mesh_p.mtxWorld._14 = 0.0f;
			g_mesh_p.mtxWorld._21 = mView._12;
			g_mesh_p.mtxWorld._22 = mView._22;
			g_mesh_p.mtxWorld._23 = mView._32;
			g_mesh_p.mtxWorld._24 = 0.0f;
			g_mesh_p.mtxWorld._31 = mView._13;
			g_mesh_p.mtxWorld._32 = mView._23;
			g_mesh_p.mtxWorld._33 = mView._33;
			g_mesh_p.mtxWorld._34 = 0.0f;
			// �ʒu�𔽉f
			g_mesh_p.mtxWorld._41 = pBullet->pos.x;
			g_mesh_p.mtxWorld._42 = pBullet->pos.y;
			g_mesh_p.mtxWorld._43 = pBullet->pos.z;
			g_mesh_p.mtxWorld._44 = 1.0f;
			// �`��
			DrawMesh(pDC, &g_mesh_p);

		}else {
			// �r���[�s��̉�]�����̓]�u�s���ݒ�
			g_mesh_e.mtxWorld._11 = mView._11;
			g_mesh_e.mtxWorld._12 = mView._21;
			g_mesh_e.mtxWorld._13 = mView._31;
			g_mesh_e.mtxWorld._14 = 0.0f;
			g_mesh_e.mtxWorld._21 = mView._12;
			g_mesh_e.mtxWorld._22 = mView._22;
			g_mesh_e.mtxWorld._23 = mView._32;
			g_mesh_e.mtxWorld._24 = 0.0f;
			g_mesh_e.mtxWorld._31 = mView._13;
			g_mesh_e.mtxWorld._32 = mView._23;
			g_mesh_e.mtxWorld._33 = mView._33;
			g_mesh_e.mtxWorld._34 = 0.0f;
			// �ʒu�𔽉f
			g_mesh_e.mtxWorld._41 = pBullet->pos.x;
			g_mesh_e.mtxWorld._42 = pBullet->pos.y;
			g_mesh_e.mtxWorld._43 = pBullet->pos.z;
			g_mesh_e.mtxWorld._44 = 1.0f;
			// �`��
			DrawMesh(pDC, &g_mesh_e);
		}

	}
	SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//====================================================================================
//
//				����
//
//====================================================================================
int FireBullet(XMFLOAT3 pos, XMFLOAT3 dir, EBulletType type)
{
	TBullet* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		// �g�p���Ȃ�X�L�b�v
		if (pBullet->use) {
			continue;
		}
		pBullet->pos = pos;
		// �����x�N�g���𐳋K��
		XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
		if (type == BULLETTYPE_PLAYER) {
			pBullet->vel.x = dir.x * BULLET_SPEED;
			pBullet->vel.y = dir.y * BULLET_SPEED;
			pBullet->vel.z = dir.z * BULLET_SPEED;
		} else {
			pBullet->vel.x = dir.x * EBULLET_SPEED;
			pBullet->vel.y = dir.y * EBULLET_SPEED;
			pBullet->vel.z = dir.z * EBULLET_SPEED;
		}
		pBullet->nShadow = CreateShadow(pos, BULLET_RADIUS);
		XMFLOAT4X4 mtxWorld;
		XMStoreFloat4x4(&mtxWorld,
			XMMatrixTranslation(pBullet->pos.x, pBullet->pos.y, pBullet->pos.z));
		pBullet->use = true;
		pBullet->type = type;
		return i;	// ���˂ł���(0�`:�e�ԍ�)
	}
	return -1;		// ���˂ł��Ȃ�����
}

//====================================================================================
//
//				���_���̍쐬
//
//====================================================================================
HRESULT MakeVertexBullet(ID3D11Device* pDevice)
{
	// �ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh_p.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh_p.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3( BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3( BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
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
	g_mesh_p.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh_p, pVertexWk, pIndexWk);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}
