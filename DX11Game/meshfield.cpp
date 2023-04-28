//************************************************************************************
// 
// ���b�V���n��[meshfield.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
//-------------------- �C���N���[�h�� --------------------
#include "Meshfield.h"
#include "Shader.h"
#include "Texture.h"


//-------------------- �萔��` --------------------
#define	TEXTURE_FILENAME	(L"data/texture/field004.jpg")		// �e�N�X�`���t�@�C����
#define	BUMP_TEXTURE		(L"data/texture/field004_bump.png")	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define M_SPECULAR			(XMFLOAT4(0.0f,0.0f,0.0f,1.0f))
#define M_POWER				(50.0f)
#define M_AMBIENT			(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define M_EMISSIVE			(XMFLOAT4(0.0f,0.0f,0.0f,1.0f))

#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_ROTATE		(0.2f)							// ��]��

MESH MeshField::mesh;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
MeshField::MeshField() {

}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
MeshField::~MeshField() {

}


//====================================================================================
//
//				������
//
//====================================================================================
HRESULT MeshField::Init(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ) {
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();
	HRESULT hr;

	// �ʒu�E��]�̏����ݒ�
	mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	material.Diffuse = M_DIFFUSE;
	material.Ambient = M_AMBIENT;
	material.Specular = M_SPECULAR;
	material.Power = M_POWER;
	material.Emissive = M_EMISSIVE;
	mesh.pMaterial = &material;

	// �e�N�X�`���̓ǂݍ���
	//if(pTexture == nullptr){
	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
	//}
	if (mesh.pTexture == nullptr) {
		hr = pTexture->SetTexture(pDevice, TEXTURE_FILENAME);
		mesh.pTexture = pTexture->GetTexture();
	}
	pTexture.reset();

	XMStoreFloat4x4(&mesh.mtxTexture, XMMatrixIdentity());

	// ���_���̍쐬
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void MeshField::Uninit(void) {
	// ���b�V�����
	ReleaseMesh(&mesh);
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void MeshField::Update(void) {
	// ���b�V���X�V
	UpdateMesh(&mesh);
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void MeshField::Draw(void) {
	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// ���b�V���̕`��
	DrawMesh(pDC, &mesh);
}

//====================================================================================
//
//				���_�̍쐬
//
//====================================================================================
HRESULT MeshField::MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ) {
	// �v���~�e�B�u��ʐݒ�
	mesh.primitiveType = PT_TRIANGLESTRIP;

	// ���_���̐ݒ�
	mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ���_�z��̍쐬
	VERTEX_3D* pVertexWk = new VERTEX_3D[mesh.nNumVertex];

	// �C���f�b�N�X�z��̍쐬
	int* pIndexWk = new int[mesh.nNumIndex];

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
	HRESULT hr = MakeMeshVertex(pDevice, &mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}
