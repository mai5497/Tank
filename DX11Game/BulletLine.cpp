#include "BulletLine.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "player.h"
#include "input.h"

//-------------------- �萔��` --------------------
#define	TEX_FILENAME	(L"data/texture/FX_BulletLine.png")	// �e�N�X�`�� �t�@�C����
#define LINE_WIDTH				(50)
#define LINE_HEIGHT				(10)

#define M_DIFFUSE				XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER					(1.0f)
#define M_AMBIENT				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE				XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


BulletLine::BulletLine() {

}

BulletLine::~BulletLine() {

}

void BulletLine::Init(GameObject *_pObj) {
	pObj = _pObj;

	ID3D11Device* pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �}�e���A���̐ݒ�
	mat.Diffuse = M_DIFFUSE;
	mat.Ambient = M_AMBIENT;
	mat.Specular = M_SPECULAR;
	mat.Power = M_POWER;
	mat.Emissive = M_EMISSIVE;
	line.pMaterial = &mat;
	// �e�N�X�`���̓ǂݍ���
	std::unique_ptr<Texture> pTexture_p = std::make_unique<Texture>();
	HRESULT isLoad_p = pTexture_p->SetTexture(pDevice, TEX_FILENAME);
	if (FAILED(isLoad_p)) {
		MessageBox(NULL, _T("�e�\�����e�N�X�`���ǂݍ��ݎ��s"), _T("error"), MB_OK);
	}
	line.pTexture = pTexture_p->GetTexture();

	XMStoreFloat4x4(&line.mtxTexture, XMMatrixIdentity());
	XMStoreFloat4x4(&line.mtxWorld, XMMatrixIdentity());

	dir = XMFLOAT3(-pObj->mtxWorld._31, -pObj->mtxWorld._32, -pObj->mtxWorld._33);
	pos = XMFLOAT3(pObj->pos.x + dir.x*LINE_WIDTH, 5.0f, pObj->pos.z + dir.z*LINE_WIDTH);
	angle = XMFLOAT3(90.0f, pObj->rotModel.y + 90.0f, pObj->rotModel.z);

	XMStoreFloat4x4(&line.mtxWorld, XMMatrixTranslation(pos.x,pos.y,pos.z));
}

void BulletLine::Uninit() {
	ReleaseMesh(&line);
}

void BulletLine::Update() {
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));	// ���K��
	pos = XMFLOAT3(pObj->pos.x  + dir.x * LINE_WIDTH , 5.0f, pObj->pos.z + dir.z * LINE_WIDTH);
	//pos = XMFLOAT3(0.0, 5.0f, 0.0);
	
	float radian = atan2f(dir.z, dir.x);
	float degree = radian * 180 / 3.14f;
	angle = XMFLOAT3(90.0f, -degree, pObj->rotModel.z);

	XMMATRIX _mtxWorld, _mtxRot, _mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	_mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	_mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(angle.x),
		XMConvertToRadians(angle.y), XMConvertToRadians(angle.z));
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxRot);

	// �ړ��𔽉f
	_mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	_mtxWorld = XMMatrixMultiply(_mtxWorld, _mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&line.mtxWorld, _mtxWorld);

}

void BulletLine::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	// ��������
	SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	SetZBuffer(true);// Z�o�b�t�@�L��

	// �`��
	DrawMesh(pDC, &line);

	// ���ɖ߂�
	SetZBuffer(false);// Z�o�b�t�@����
	SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//====================================================================================
//
//				�p�x��ۑ�
//
//====================================================================================
void BulletLine::SetDir(XMFLOAT3 _dir) {
	dir = _dir;
}

//====================================================================================
//
//				���_���̍쐬
//
//====================================================================================
void BulletLine::MakeVertexBullet(ID3D11Device* pDevice) {
	// �ꎞ�I�Ȓ��_�z��𐶐�
	line.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[line.nNumVertex];

	line.primitiveType = PT_TRIANGLESTRIP;

	// ���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-LINE_WIDTH, LINE_HEIGHT, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(LINE_WIDTH, LINE_HEIGHT, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-LINE_WIDTH, -LINE_HEIGHT, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(LINE_WIDTH, -LINE_HEIGHT, 0.0f);
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
	line.nNumIndex = 4;
	int* pIndexWk = new int[4];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	MakeMeshVertex(pDevice, &line, pVertexWk, pIndexWk);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
}
