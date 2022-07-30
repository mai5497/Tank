//=============================================================================
//
// �������� [explosion.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "DwarfEffect.h"
#include "Texture.h"
#include "mesh.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME		L"data/texture/blue_noom.png"

#define M_DIFFUSE				XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER					(1.0f)
#define M_AMBIENT				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_DWARF			(100)

#define DWARF_FRAME_X		(10)		// ���t���[����(�e�N�X�`���ɍ�)
#define DWARF_FRAME_Y		(6)		// �c�t���[����(�킹�ĕύX����)
#define DWARF_ANIM_COUNT	(1)		// ��R�}������̕\����

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TDwarf {
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT2	size;		// �T�C�Y
	bool		use;		// �g�p���Ă��邩�ǂ���
	int			anim;		// �A�j���[�V�����Đ��ʒu
	int			count;		// �A�j���[�V���� �t���[����
	XMFLOAT4	color;		// �F
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH			g_mesh;						// ���b�V�����
static MATERIAL		g_material;					// �}�e���A��
static TDwarf	g_explosion[MAX_DWARF];	// �������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexDwarf(ID3D11Device* pDevice);

//=============================================================================
// ������
//=============================================================================
HRESULT InitDwarfEffect(void) {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// ���_���̍쐬
	hr = MakeVertexDwarf(pDevice);
	if (FAILED(hr)) {
		return hr;
	}

	// �}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr)) {
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ���[���h �}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// ������񏉊���
	for (int i = 0; i < MAX_DWARF; ++i) {
		g_explosion[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_explosion[i].size = XMFLOAT2(10.0f, 10.0f);
		g_explosion[i].use = false;
		g_explosion[i].anim = 0;
		g_explosion[i].count = DWARF_ANIM_COUNT;
		g_explosion[i].color = M_DIFFUSE;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDwarfEffect(void) {
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V
//=============================================================================
void UpdateDwarfEffect(void) {
	TDwarf* pDwarf = g_explosion;
	for (int i = 0; i < MAX_DWARF; ++i, ++pDwarf) {
		// ���g�p�Ȃ�X�L�b�v
		if (!pDwarf->use) {
			continue;
		}
		// �J�E���^�X�V
		--pDwarf->count;
		if (pDwarf->count <= 0) {
			// �A�j���[�V�����X�V
			++pDwarf->anim;
			if (pDwarf->anim >= DWARF_FRAME_X * DWARF_FRAME_Y) {
				pDwarf->use = false;	// ����
				continue;
			}
			// �J�E���^������
			pDwarf->count = DWARF_ANIM_COUNT;
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void DrawDwarfEffect(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// ��������
	SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TDwarf* pDwarf = g_explosion;
	for (int i = 0; i < MAX_DWARF; ++i, ++pDwarf) {
		// ���g�p�Ȃ�X�L�b�v
		if (!pDwarf->use) {
			continue;
		}
		// �e�N�X�`�� �}�g���b�N�X�X�V
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / DWARF_FRAME_X,
			1.0f / DWARF_FRAME_Y, 1.0f);
		int u = pDwarf->anim % DWARF_FRAME_X;
		int v = pDwarf->anim / DWARF_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex,
			XMMatrixTranslation((float)u / DWARF_FRAME_X,
			(float)v / DWARF_FRAME_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		// �r���[�s��̉�]�����̓]�u�s���ݒ�(+�g�k)
		g_mesh.mtxWorld._11 = mView._11 * pDwarf->size.x;
		g_mesh.mtxWorld._12 = mView._21 * pDwarf->size.x;
		g_mesh.mtxWorld._13 = mView._31 * pDwarf->size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pDwarf->size.y;
		g_mesh.mtxWorld._22 = mView._22 * pDwarf->size.y;
		g_mesh.mtxWorld._23 = mView._32 * pDwarf->size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pDwarf->pos.x;
		g_mesh.mtxWorld._42 = pDwarf->pos.y;
		g_mesh.mtxWorld._43 = pDwarf->pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		// �F�𔽉f
		g_material.Diffuse = pDwarf->color;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//=============================================================================
// �����J�n
//=============================================================================
int StartDwarfEffect(XMFLOAT3 pos, XMFLOAT2 size) {
	pos.y += 50.0f;
	TDwarf* pDwarf = g_explosion;
	for (int i = 0; i < MAX_DWARF; ++i, ++pDwarf) {
		// �g�p���Ȃ�X�L�b�v
		if (pDwarf->use) {
			continue;
		}
		pDwarf->pos = pos;
		pDwarf->size = size;
		pDwarf->anim = 0;
		pDwarf->count = DWARF_ANIM_COUNT;
		pDwarf->use = true;
		pDwarf->color = M_DIFFUSE;
		return i;	// ��������(0�`:�����ԍ�)
	}
	return -1;		// �������Ȃ�����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexDwarf(ID3D11Device* pDevice) {
	// �ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
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
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void SetDwarfColor(int nDwarf, XMFLOAT4 color) {
	if (nDwarf >= 0 && nDwarf < MAX_DWARF) {
		TDwarf* pExp = &g_explosion[nDwarf];
		pExp->color = color;
	}
}
