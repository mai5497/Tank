//=============================================================================
//
// �ۉe [shadow.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "shadow.h"
#include "mesh.h"
#include "Texture.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/shadow000.jpg"	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_SHADOW			(5000)

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct {
	int			stat;	// ���
	XMFLOAT3	pos;	// �ʒu
	float		radius;	// ���a
} SHADOW;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static MESH							g_mesh;					// �\����
static MATERIAL						g_material;				// �}�e���A��
static SHADOW						g_shadow[MAX_SHADOW];	// �ۉe���

// ������
HRESULT InitShadow(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr))
		return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// ���_���̍쐬
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.0f,  0.5f);
	pVertexWk[1].vtx = XMFLOAT3( 0.5f, 0.0f,  0.5f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, 0.0f, -0.5f);
	pVertexWk[3].vtx = XMFLOAT3( 0.5f, 0.0f, -0.5f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

// �I������
void UninitShadow(void)
{
	ReleaseMesh(&g_mesh);
}

// �X�V
void UpdateShadow(void)
{
	// �������Ȃ�
}

// �`��
void DrawShadow(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CLight::Get()->SetDisable();
	SetBlendState(BS_SUBTRACTION);	// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	SHADOW* pShadow = g_shadow;
	for (int i = 0; i < MAX_SHADOW; ++i, ++pShadow) {
		if (!pShadow->stat) continue;
		float ratio = pShadow->pos.y / 320.0f;
		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;
		// �T�C�Y�𔽉f
		g_mesh.mtxWorld._11 =
		g_mesh.mtxWorld._33 = pShadow->radius * 2 * (1.0f - ratio);
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pShadow->pos.x;
		g_mesh.mtxWorld._42 = 1.0f;	// Z�t�@�C�e�B���O����̂���0.0f�����
		g_mesh.mtxWorld._43 = pShadow->pos.z;
		// �s�����x�𔽉f
		g_material.Diffuse.w = 0.5f - ratio;
		if (g_material.Diffuse.w < 0.0f)
			g_material.Diffuse.w = 0.0f;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

// ����
int CreateShadow(XMFLOAT3 pos, float radius)
{
	SHADOW* pShadow = g_shadow;
	for (int i = 0; i < MAX_SHADOW; ++i, ++pShadow) {
		if (pShadow->stat) continue;
		pShadow->pos = pos;
		pShadow->radius = radius;
		pShadow->stat = 1;
		return i;
	}
	return -1;
}

// �ړ�
void MoveShadow(int nShadow, XMFLOAT3 pos)
{
	if (nShadow < 0 || nShadow >= MAX_SHADOW)
		return;
	g_shadow[nShadow].pos = pos;
}

// ���
void ReleaseShadow(int nShadow)
{
	if (nShadow < 0 || nShadow >= MAX_SHADOW)
		return;
	g_shadow[nShadow].stat = 0;
}
