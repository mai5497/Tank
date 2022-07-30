//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "effect.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT			L"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	EFFECT_SIZE_Y			(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BULLET		(2.0f)							// �ړ����x

#define	MAX_EFFECT				(4096)							// �G�t�F�N�g�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TEffect {
	XMFLOAT3 pos;			// �ʒu
	XMFLOAT3 move;			// �ړ���
	XMFLOAT4 col;			// �F
	XMFLOAT2 size;			// �傫��
	int nTimer;				// �^�C�}�[
	float fDecAlpha;		// �����l
	bool use;				// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(ID3D11Device* pDevice);
void SetVertexEffect(int nIdxEffect, XMFLOAT2 size);
void SetColorEffect(int nIdxEffect, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static TEffect			g_effect[MAX_EFFECT];	// �G�t�F�N�g���
static bool				g_burstFlg;				// ���U�t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device* pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexEffect(pDevice);

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
						  TEXTURE_EFFECT,		// �t�@�C���̖��O
						  &g_mesh.pTexture);	// �ǂݍ��ރ������[
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_mesh.pMaterial = &g_material;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; ++nCntEffect) {
		g_effect[nCntEffect].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_effect[nCntEffect].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_effect[nCntEffect].size = XMFLOAT2(EFFECT_SIZE_X, EFFECT_SIZE_Y);
		g_effect[nCntEffect].nTimer = 0;
		g_effect[nCntEffect].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	// ���b�V���̊J��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; ++nCntEffect) {
		if (g_effect[nCntEffect].use) {
			g_effect[nCntEffect].pos.x += g_effect[nCntEffect].move.x;
			g_effect[nCntEffect].pos.y += g_effect[nCntEffect].move.y;
			g_effect[nCntEffect].pos.z += g_effect[nCntEffect].move.z;

			g_effect[nCntEffect].col.w -= g_effect[nCntEffect].fDecAlpha;
			if (g_effect[nCntEffect].col.w <= 0.0f) {
				g_effect[nCntEffect].col.w = 0.0f;
			}
			SetColorEffect(nCntEffect, g_effect[nCntEffect].col);

			--g_effect[nCntEffect].nTimer;
			if (g_effect[nCntEffect].nTimer <= 0) {
				g_effect[nCntEffect].use = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);		// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; ++nCntEffect) {
		if (g_effect[nCntEffect].use) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

			g_mesh.mtxWorld._11 = mtxView._11;
			g_mesh.mtxWorld._12 = mtxView._21;
			g_mesh.mtxWorld._13 = mtxView._31;
			g_mesh.mtxWorld._21 = mtxView._12;
			g_mesh.mtxWorld._22 = mtxView._22;
			g_mesh.mtxWorld._23 = mtxView._32;
			g_mesh.mtxWorld._31 = mtxView._13;
			g_mesh.mtxWorld._32 = mtxView._23;
			g_mesh.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_mesh.mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(g_effect[nCntEffect].size.x, g_effect[nCntEffect].size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_effect[nCntEffect].pos.x, g_effect[nCntEffect].pos.y, g_effect[nCntEffect].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

			// �J���[�̐ݒ�
			g_material.Diffuse = g_effect[nCntEffect].col;

			// ���b�V���̕`��
			DrawMesh(pDC, &g_mesh);
		}
	}

	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEffect(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	// �C���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEffect(int nIdxEffect, XMFLOAT2 size)
{
	g_effect[nIdxEffect].size = size;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorEffect(int nIdxEffect, XMFLOAT4 col)
{
	g_effect[nIdxEffect].col = col;
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; ++nCntEffect) {
		if (!g_effect[nCntEffect].use) {
			g_effect[nCntEffect].pos = pos;
			g_effect[nCntEffect].move = move;
			g_effect[nCntEffect].nTimer = nTimer;
			g_effect[nCntEffect].fDecAlpha = col.w / nTimer;
			g_effect[nCntEffect].use = true;

			// ���_���W�̐ݒ�
			SetVertexEffect(nCntEffect, size);

			// ���_�J���[�̐ݒ�
			SetColorEffect(nCntEffect, col);

			nIdxEffect = nCntEffect;

			break;
		}
	}

	return nIdxEffect;
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetEffect(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT2 size, int nTimer) {
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < 100; ++nCntEffect) {
		if (!g_effect[nCntEffect].use) {
			g_effect[nCntEffect].pos = pos;
			g_effect[nCntEffect].nTimer = nTimer;
			g_effect[nCntEffect].fDecAlpha = col.w / nTimer;
			g_effect[nCntEffect].use = true;
			g_effect[nCntEffect].move.x = rand() % 50 - 25;	// -25~25�̗���
			g_effect[nCntEffect].move.y = ((rand() % 100) * 2.0f * 0.05f);
			g_effect[nCntEffect].move.z = rand() % 50 - 25;	// -25~25�̗���




			// ���_���W�̐ݒ�
			SetVertexEffect(nCntEffect, size);

			// ���_�J���[�̐ݒ�
			SetColorEffect(nCntEffect, col);

			nIdxEffect = nCntEffect;

			//break;
		}
	}

	return nIdxEffect;
}

// ���U�t���O
void SetBurstFlg(bool flg) {
	g_burstFlg = flg;	// �t���O�̃I���I�t
}


// ���U�G�t�F�N�g�J�n
void StartBurst() {

}