//=============================================================================
//
// ���b�V���n�ʏ��� [meshfield.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "meshfield.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/field004.jpg"	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_ROTATE		(0.2f)							// ��]��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static MESH							g_mesh;					// �\����
static MATERIAL						g_material;				// �}�e���A��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// �ʒu�E��]�̏����ݒ�
	g_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

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

	// ���_���̍쐬
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	// ���b�V�����
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
	// ���b�V���X�V
	UpdateMesh(&g_mesh);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// ���b�V���̕`��
	DrawMesh(pDC, &g_mesh);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ)
{
	// �v���~�e�B�u��ʐݒ�
	g_mesh.primitiveType = PT_TRIANGLESTRIP;

	// ���_���̐ݒ�
	g_mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	g_mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ���_�z��̍쐬
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// �C���f�b�N�X�z��̍쐬
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	for (int z = 0; z < nNumBlockZ + 1; ++z) {
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			// ���_���W�̐ݒ�
			pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
			pVtx->vtx.y = 0.0f;
			pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;

			// �@���̐ݒ�
			pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//�C���f�b�N�X�z��̒��g�𖄂߂�
	int* pIdx = pIndexWk;
	for (int z = 0; z < nNumBlockZ; ++z) {
		if (z > 0) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			*pIdx++ = (z + 1) * (nNumBlockX + 1);
		}
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
			*pIdx++ = z * (nNumBlockX + 1) + x;
		}
		if (z < nNumBlockZ - 1) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
		}
	}

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}
