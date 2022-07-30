//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "meshwall.h"
#include "mesh.h"
#include "Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/wall000.jpg"	// �e�N�X�`���t�@�C����
#define	MAX_MESHWALL		(32)						// �ǂ̑���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;	// �e�N�X�`���ǂݍ��ݏꏊ
static MESH			g_meshWall[MAX_MESHWALL];	// ���b�V���ǃ��[�N
static int			g_nNumMeshWall = 0;			// ���b�V���ǂ̐�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshWall(void)
{
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	HRESULT hr = CreateTextureFromFile(pDevice,				// �f�o�C�X �|�C���^
									   TEXTURE_FILENAME,	// �t�@�C����
									   &g_pTexture);		// �ǂݍ��ރ�����

	return hr;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock)
{
	ID3D11Device* pDevice = GetDevice();
	MESH* pMesh;

	if (g_nNumMeshWall >= MAX_MESHWALL) {
		return E_FAIL;
	}

	pMesh = &g_meshWall[g_nNumMeshWall];
	g_nNumMeshWall++;

	// �e�N�X�`����ݒ�
	XMStoreFloat4x4(&pMesh->mtxTexture, XMMatrixIdentity());
	pMesh->pTexture = g_pTexture;

	// �|���S���\���ʒu�̒��S���W�A�p�x��ݒ�
	pMesh->pos = pos;
	pMesh->rot = rot;

	// ���_���̐ݒ�
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh->nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	VERTEX_3D* pVertexWk = new VERTEX_3D[pMesh->nNumVertex];

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	int* pIndexWk = new int[pMesh->nNumIndex];

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;
#if 0
	const float texSizeX = 1.0f / nNumBlockX;
	const float texSizeZ = 1.0f / nNumBlockY;
#else
	const float texSizeX = 1.0f;
	const float texSizeZ = 1.0f;
#endif
	for (int nCntVtxY = 0; nCntVtxY < nNumBlockY + 1; ++nCntVtxY) {
		for (int nCntVtxX = 0; nCntVtxX < nNumBlockX + 1; ++nCntVtxX) {
			// ���_���W�̐ݒ�
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.x = -(nNumBlockX / 2.0f) * sizeBlock.x + nCntVtxX * sizeBlock.x;
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.y = nNumBlockY * sizeBlock.y - nCntVtxY * sizeBlock.y;
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.z = 0.0f;

			// �@���̐ݒ�
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].diffuse = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
			pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxY;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
	int* pIdx = pIndexWk;

	int nCntIdx = 0;
	for (int nCntVtxY = 0; nCntVtxY < nNumBlockY; ++nCntVtxY) {
		if (nCntVtxY > 0) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			pIdx[nCntIdx] = (nCntVtxY + 1) * (nNumBlockX + 1);
			++nCntIdx;
		}

		for (int nCntVtxX = 0; nCntVtxX < nNumBlockX + 1; ++nCntVtxX) {
			pIdx[nCntIdx] = (nCntVtxY + 1) * (nNumBlockX + 1) + nCntVtxX;
			++nCntIdx;
			pIdx[nCntIdx] = nCntVtxY * (nNumBlockX + 1) + nCntVtxX;
			++nCntIdx;
		}

		if (nCntVtxY < nNumBlockY - 1) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			pIdx[nCntIdx] = nCntVtxY * (nNumBlockX + 1) + nNumBlockX;
			++nCntIdx;
		}
	}

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	MakeMeshVertex(pDevice, pMesh, pVertexWk, pIndexWk);

	// �ꎞ�z����
	delete[] pIndexWk;
	delete[] pVertexWk;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	MESH* pMesh;

	// �S���b�V���J��
	for (int i = 0; i < g_nNumMeshWall; ++i) {
		pMesh = &g_meshWall[i];
		pMesh->pTexture = nullptr;
		ReleaseMesh(pMesh);
	}
	g_nNumMeshWall = 0;

	// �e�N�X�`���̊J��
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
	for (int i = 0; i < g_nNumMeshWall; ++i) {
		// ���b�V���X�V
		UpdateMesh(&g_meshWall[i]);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(EDrawPart dp)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	MESH* pWall = g_meshWall;
	for (int i = 0; i < g_nNumMeshWall; ++i, ++pWall) {
		// ���b�V���`��
		switch (dp) {
		case DRAWPART_OPAQUE:		// �s�����̂�
			if (pWall->fAlpha < 1.0f)
				continue;
			SetBlendState(BS_NONE);
			SetZWrite(false);
			break;
		case DRAWPART_TRANSLUCENT:	// �������̂�
			if (pWall->fAlpha >= 1.0f)
				continue;
			SetBlendState(BS_ALPHABLEND);
			SetZBuffer(true);
			break;
		default:
			if (pWall->fAlpha < 1.0f) {
				SetBlendState(BS_NONE);
				SetZWrite(false);
			} else {
				SetBlendState(BS_ALPHABLEND);
				SetZBuffer(true);
			}
			break;
		}
		DrawMesh(pDC, pWall);
	}
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}
