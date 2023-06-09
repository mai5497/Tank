//************************************************************************************
// 
// �^�C�g�����S[TitleLogo.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "TitleLogo.h"

//-------------------- �萔��` --------------------
#define TEXTURE_PATH			(L"data/texture/UI_TitleLogo.png")
#define TEXTURE_WIDTH			(900)
#define TEXTURE_HEIGHT			(150)

#define TEXTURE_POSX_BASE		(-375.0f)


#define SPRING_DEFAULT_LENGTH	(2.0f)			// �o�l�̎��R��
#define SPRING_COEFFICIENT		(1.0f)			// �o�l�W��
#define SPRING_MASS				(10.0f)			// �o�l�̎���
#define ATT_COEFFICIENT			(1.0f)			// �����W��(Attenuation)


#define F_ZERO(v)			v.x=v.y=0.0f
#define F_SET(v,px,py)		v.x=px;v.y=py
#define F_SQRT(v)			sqrtf(v.x*v.x+v.y*v.y)

std::unique_ptr<Texture> TitleLogo::pTexture;
LPCWSTR TitleLogo::pszTexFName;


//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
TitleLogo::TitleLogo() {

}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
TitleLogo::~TitleLogo() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void TitleLogo::Init() {
	pszTexFName = TEXTURE_PATH;

	ID3D11Device* pDevice = GetDevice();

	pTexture = std::make_unique<Texture>();
	pTexture->SetTexture(pDevice, pszTexFName);

	for (int j = 0; j < STRING_TOTAL; j++) {
		float massRate = (rand() % 10) * 0.1 + 0.1f;
		for (int i = 0; i < SPRING_TOTAL; i++) {
			springs[j][i].status = 0;		// �΂˂̏�����
			springs[j][i].mass = SPRING_MASS * massRate;
		}
	}

}

//====================================================================================
//
//				���Z�b�g
//
//====================================================================================
void TitleLogo::Reset() {
	for (int j = 0; j < STRING_TOTAL; j++) {
		for (int i = 0; i < SPRING_TOTAL; i++) {
			springs[j][i].status = 0;		// �΂˂̏�����
		}
	}
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void TitleLogo::Uninit() {
	// �e�N�X�`���J��
	pTexture->ReleaseTexture();
	pTexture.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void TitleLogo::Update() {
	//----- �ϐ������� -----
	float length;			// �o�l�̒���
	float posx;				// �������p���W�v�Z
	XMFLOAT2 vec_up;		// �㑤�̗�
	XMFLOAT2 vec_down;		// �����̗�
	XMFLOAT2 resistance;	// ��R��
	float now_tension;		// �e���͂̑傫��
	int	i,j;

	//----- �΂˂̏��� -----
	for (j = 0; j < STRING_TOTAL; j++) {
		for (i = 0; i < SPRING_TOTAL; i++) {
			switch (springs[j][i].status) {
			case 0:			// ������
				posx = TEXTURE_POSX_BASE + j * (TEXTURE_WIDTH / STRING_TOTAL);

				F_SET(springs[j][i].position, posx, -SPRING_DEFAULT_LENGTH * i + TEXTURE_HEIGHT);	// ���W
				F_ZERO(springs[j][i].velocity);														// ���x
				F_ZERO(springs[j][i].acceleration);													// �����x
				F_ZERO(springs[j][i].tension_up);													// �e����
				F_ZERO(springs[j][i].tension_down);													// �e����
				F_ZERO(springs[j][i].resultant);													// ����
				F_SET(springs[j][i].gravity, 0.0f, springs[j][i].mass * -0.98f);					// �d��
				springs[j][i].onLock = (i == 0);													// ��[�̓��b�N
				springs[j][i].status = 1;
				break;
			case 1:
				// �P�F�㑤�Ɉ���������͂����߂�
				// �㑤�ɐڑ�����Ă���΂˂ɂ��Ēe�͐������߂�
				if (i == 0) {
					springs[j][i].tension_up.x = 0.0f;	// �ŏ㕔�Ȃ̂ŏォ��̗͂͂O
					springs[j][i].tension_up.y = 0.0f;
				} else {
					vec_up.x = springs[j][i - 1].position.x - springs[j][i].position.x;	// ������̃x�N�g�������߂�
					vec_up.y = springs[j][i - 1].position.y - springs[j][i].position.y;	// ������̃x�N�g�������߂�
					length = sqrt(vec_up.x * vec_up.x + vec_up.y * vec_up.y);
					if (length == 0) {
						springs[j][i].tension_up.x = 0.0f;	// �������O�Ȃ�͂��O
						springs[j][i].tension_up.y = 0.0f;
					} else {
						now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);	// �萔�Ǝ��R������v�Z
						springs[j][i].tension_up.x = vec_up.x * (now_tension / length);	// ������̗͂������x�N�g�����v�Z
						springs[j][i].tension_up.y = vec_up.y * (now_tension / length);
					}
				}


				// �Q�F�����Ɉ���������͂����߂�
				if (i == SPRING_TOTAL - 1) {
					springs[j][i].tension_down.x = 0.0f;	// �ŉ����Ȃ̂ŉ�����̗͂͂O
					springs[j][i].tension_down.y = 0.0f;
				} else {
					vec_down.x = springs[j][i + 1].position.x - springs[j][i].position.x;	// �������̃x�N�g�������߂�
					vec_down.y = springs[j][i + 1].position.y - springs[j][i].position.y;
					length = sqrt(vec_down.x * vec_down.x + vec_down.y * vec_down.y);
					if (length == 0) {
						springs[j][i].tension_down.x = 0.0f;	// �������O�Ȃ�͂��O
						springs[j][i].tension_down.y = 0.0f;
					} else {
						now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);	// �萔�Ǝ��R������v�Z
						springs[j][i].tension_down.x = vec_down.x * (now_tension / length);	// �������̗͂������x�N�g�����v�Z
						springs[j][i].tension_down.y = vec_down.y * (now_tension / length);
					}
				}

				// �R�F�d�͂����߂�
				springs[j][i].gravity.x = 0.0f;
				springs[j][i].gravity.y = springs[j][i].mass * -0.98f;	// �΂˒萔�Əd�͉����x

				// �S�F��R�͂����߂�
				resistance.x = -springs[j][i].velocity.x * ATT_COEFFICIENT;	// ���x�̋t�����ɒ�R�͒萔��������
				resistance.y = -springs[j][i].velocity.y * ATT_COEFFICIENT;

				// �T�F�P����S�ŋ��߂��͂��������č��͂����߂�
				springs[j][i].resultant.x = springs[j][i].gravity.x + springs[j][i].tension_up.x + springs[j][i].tension_down.x + resistance.x;
				springs[j][i].resultant.y = springs[j][i].gravity.y + springs[j][i].tension_up.y + springs[j][i].tension_down.y + resistance.y;

				if (springs[j][i].onLock == false) {
					// �U�F���͂Ǝ��ʂ�������x�����߂�
					springs[j][i].acceleration.x = springs[j][i].resultant.x / springs[j][i].mass;
					springs[j][i].acceleration.y = springs[j][i].resultant.y / springs[j][i].mass;

					// �V�F�����x���瑬�x�����߂�
					springs[j][i].velocity.x += springs[j][i].acceleration.x;
					springs[j][i].velocity.y += springs[j][i].acceleration.y;

					// �W�F���x����ʒu�����߂�
					springs[j][i].position.x += springs[j][i].velocity.x;
					springs[j][i].position.y += springs[j][i].velocity.y;
				}

				break;
			}
		}
	}

}



//====================================================================================
//
//				�`��
//
//===================================================================================
void TitleLogo::Draw() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	SetPolygonFrameSize(1.0f / STRING_TOTAL, 1.0f);

	for (int i = 0; i < STRING_TOTAL; i++) {
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetPolygonSize(TEXTURE_WIDTH/STRING_TOTAL, TEXTURE_HEIGHT);
		SetPolygonPos(springs[i][SPRING_TOTAL - 1].position.x, springs[i][SPRING_TOTAL - 1].position.y);
		SetPolygonTexture(pTexture->GetTexture());
		SetPolygonUV((i % STRING_TOTAL) / (float)STRING_TOTAL, 0.0f);

		DrawPolygon(pDC);
	}

	//----- ���ɖ߂� -----
	// Z�o�b�t�@����(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	SetPolygonFrameSize(1.0f , 1.0f);

}