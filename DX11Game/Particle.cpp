//************************************************************************************
// 
// �p�[�e�B�N��[Particle.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Particle.h"
#include "input.h"
#include "Light.h"
#include "Camera.h"

#include "polygon.h"

//-------------------- �萔��` --------------------
#define PATH_BGTEXTURE			(L"data/texture/effect000.jpg")

#define PARTICLE_POSX			(0)			// �p�[�e�B�N���\����ʒu
#define PARTICLE_POSY			(-200.0f)	
#define PARTICLE_SIZE			(100)		// �p�[�e�B�N���̍ő�̑傫��
#define PARTICLE_DISP			(0.2)		// �p�[�e�B�N���̎U��

#define PARTICLE_TIME			(120)		// ������܂ł̎���


#define MATERIAL_DIFFUSE		XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

std::unique_ptr<Texture> Particle::pTexture;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Particle::Particle() {

}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Particle::~Particle() {

}

//====================================================================================
//
//				������
//
//====================================================================================
void Particle::Init() {
	//----- �ϐ������� -----
	isButtonOnce = false;


	//----- �p�[�e�B�N�������� -----
	ID3D11Device* pDevice = GetDevice();
	float sizeRate = 0.0f;

	for (int i = 0; i < PARTICLE_MAX; i++) {
		particles[i].status = 0;	// �X�e�[�^�X��������

		sizeRate = (rand() % 9) * 0.1 + 0.1;
		particles[i].size = PARTICLE_SIZE * sizeRate;

		particles[i].acceleration.x = (rand() % 9) * 0.001 + 0.001;
		particles[i].acceleration.y = (rand() % 9) * 0.001 + 0.001;

		particles[i].timer = PARTICLE_TIME;

		// �I�u�W�F�N�g�̒��_�z��𐶐�
		VERTEX_3D* pVertexWk = new VERTEX_3D[4];

		// ���_�z��̒��g�𖄂߂�
		VERTEX_3D* pVtx = pVertexWk;

		// ���_���W�̐ݒ�
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

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
		int* pIndexWk = new int[4];

		// �C���f�b�N�X�z��̒��g�𖄂߂�
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		// ���_�o�b�t�@����
		pTexture = std::make_unique<Texture>();
		HRESULT  hr = pTexture->SetTexture(pDevice, PATH_BGTEXTURE);

		particles[i].color = XMFLOAT4((rand() % 9) * 0.1 + 0.1, (rand() % 9) * 0.1 + 0.1, (rand() % 9) * 0.1 + 0.1, 0.8f);
	}
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Particle::Uninit() {
	// �e�N�X�`���̊J��
	pTexture->ReleaseTexture();
	pTexture.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Particle::Update() {
	//----- �ϐ������� -----
	float posx, posy;
	bool bBorn = false;

	//----- ���͂���������J�n -----
	if (GetKeyRelease(VK_SPACE)) {
		if (!isButtonOnce) {
			for (int i = 0; i < PARTICLE_MAX; i++) {
				particles[i].status = 1;
			}
		}
		isButtonOnce = true;	// ��x���������������̂�true�ɂ��ē��ڈȍ~�̓X���[
	}

	for (int i = 0; i < PARTICLE_MAX; i++) {
		switch (particles[i].status) {
			case 0:					// �ҋ@���
				break;
			case 1:
				particles[i].pos.x = PARTICLE_POSX;
				particles[i].pos.y = PARTICLE_POSY;
				posx = (i * PARTICLE_SIZE * (PARTICLE_MAX / 2)) - PARTICLE_SIZE * (PARTICLE_MAX / 4);
				particles[i].velocity.x = (posx)*particles[i].acceleration.x;
				posy = (i * PARTICLE_SIZE * (PARTICLE_MAX / 2)) - PARTICLE_SIZE * (PARTICLE_MAX / 4);
				particles[i].velocity.y = (posy)*particles[i].acceleration.y;
				particles[i].status = 2;

				break;
			case 2:
				particles[i].pos.x += particles[i].velocity.x;
				particles[i].pos.y += particles[i].velocity.y;

				particles[i].timer--;
				if (particles[i].timer < 1) {
					particles[i].status = 0;
				}
				break;
		}
	}
}


//====================================================================================
//
//				�@�`��
//
//====================================================================================
void Particle::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);		// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (particles[i].status == 2) {
			SetPolygonSize(particles[i].size, particles[i].size);
			SetPolygonPos(particles[i].pos.x, particles[i].pos.y);
			SetPolygonTexture(pTexture->GetTexture());
			SetPolygonUV(0.0f, 0.0f);

			SetPolygonColor(particles[i].color.x, particles[i].color.y, particles[i].color.z);
			SetPolygonAlpha(particles[i].color.z);
			DrawPolygon(pDC);
		}
	}

	//----- ���ɖ߂� -----
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}