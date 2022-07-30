//=============================================================================
//
// ������ [smoke.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "smoke.h"
#include "Texture.h"
#include "mesh.h"
#include "shadow.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SMOKE		L"data/texture/smoke000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SMOKE_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	SMOKE_SIZE_Y		(50.0f)							// �r���{�[�h�̍���

#define	MAX_SMOKE			(512)							// �r���{�[�h�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TSmoke {
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT3	vel;		// ���x
	XMFLOAT4	col;		// �F
	XMFLOAT2	size;		// �傫��
	float		fAlpha;		// �A���t�@�l
	bool		use;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSmoke(ID3D11Device* pDevice);
void SetVertexSmoke(int nIdxBullet, XMFLOAT2 size);
void SetColorSmoke(int nIdxSmoke, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH			g_mesh;
static MATERIAL		g_material;
static TSmoke		g_smoke[MAX_SMOKE];	// �����[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSmoke(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	ZeroMemory(&g_mesh, sizeof(MESH));

	// ���_���̍쐬
	MakeVertexSmoke(pDevice);

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							   TEXTURE_SMOKE,		// �t�@�C����
							   &g_mesh.pTexture);	// �ǂݍ��ރ�����
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_mesh.pMaterial = &g_material;

	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		pSmoke->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pSmoke->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pSmoke->size = XMFLOAT2(SMOKE_SIZE_X, SMOKE_SIZE_Y);
		pSmoke->fAlpha = 1.0f;
		pSmoke->use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSmoke(void)
{
	// ���b�V���̊J��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSmoke(void)
{
	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		if (pSmoke->use) {
			// �ړ�
			pSmoke->pos.x += pSmoke->vel.x;
			pSmoke->pos.y += pSmoke->vel.y;
			pSmoke->pos.z += pSmoke->vel.z;

			// ���_���W�̐ݒ�
			pSmoke->size.x += 0.1f;
			pSmoke->size.y += 0.1f;
			SetVertexSmoke(i, pSmoke->size);

			// ���_�J���[�̐ݒ�
			pSmoke->fAlpha -= 0.002f;
			if (pSmoke->fAlpha < 0.0f) {
				pSmoke->fAlpha = 0.0f;
				pSmoke->use = false;
				continue;
			}
			SetColorSmoke(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pSmoke->fAlpha));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSmoke(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	CLight::Get()->SetDisable();
	//SetBlendState(BS_SUBTRACTION);	// ���Z����
	SetBlendState(BS_ALPHABLEND);		// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		if (pSmoke->use) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pSmoke->mtxWorld, mtxWorld);

			pSmoke->mtxWorld._11 = mtxView._11;
			pSmoke->mtxWorld._12 = mtxView._21;
			pSmoke->mtxWorld._13 = mtxView._31;
			pSmoke->mtxWorld._21 = mtxView._12;
			pSmoke->mtxWorld._22 = mtxView._22;
			pSmoke->mtxWorld._23 = mtxView._32;
			pSmoke->mtxWorld._31 = mtxView._13;
			pSmoke->mtxWorld._32 = mtxView._23;
			pSmoke->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pSmoke->mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(pSmoke->size.x, pSmoke->size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(pSmoke->pos.x, pSmoke->pos.y, pSmoke->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&pSmoke->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pSmoke->mtxWorld;

			// �J���[�̐ݒ�
			g_material.Diffuse = pSmoke->col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_mesh);
		}
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexSmoke(ID3D11Device* pDevice)
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
void SetVertexSmoke(int nIdxSmoke, XMFLOAT2 size)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		TSmoke* pSmoke = &g_smoke[nIdxSmoke];
		pSmoke->size = size;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorSmoke(int nIdxSmoke, XMFLOAT4 col)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		g_smoke[nIdxSmoke].col = col;
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetSmoke(XMFLOAT3 pos, XMFLOAT2 size)
{
	int nIdxSmoke = -1;

	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		if (!pSmoke->use) {
			pSmoke->pos = pos;
			XMMATRIX mRot = XMMatrixRotationY(XMConvertToRadians((float)(rand() % 360)));
			XMVECTOR vAxis = XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), mRot);
			XMStoreFloat3(&pSmoke->vel, XMVector3TransformNormal(XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f), XMMatrixRotationAxis(vAxis, XMConvertToRadians(10))));
			pSmoke->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pSmoke->fAlpha = 0.2f;
			pSmoke->use = true;

			// ���_���W�̐ݒ�
			SetVertexSmoke(i, size);

			nIdxSmoke = i;
			break;
		}
	}

	return nIdxSmoke;
}
