//----- �C���N���[�h�� -----
#include "timer.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"
#include "debugproc.h"

//----- �萔��` -----
#define MIN_CHRCNT				(2)												// ��������
#define SEC_CHRCNT				(2)												// �b������
#define TIMER_POS_Y				(360.0f - 20.0f)								// �\���ʒu

#define TIMERROGO_POS_X			(0 - (MIN_CHRCNT + 1) * NUMBER_SIZE_X)			// �^�C�}�[���S�ʒu
#define TIMERROGO_POS_Y			(TIMER_POS_Y - NUMBER_SIZE_Y * 0.5)

#define MINCHR_POS_X			(0.0f)															// �������ʒu
#define MINNUM_POS_X			(MINCHR_POS_X - MIN_CHRCNT * NUMBER_SIZE_X - NUMBER_SIZE_X / 2)	// �������ʒu
#define SECNUM_POS_X			(MINCHR_POS_X + NUMBER_SIZE_X - NUMBER_SIZE_X / 2)
#define SECCHR_POS_X			(SECNUM_POS_X + SEC_CHRCNT * NUMBER_SIZE_X + NUMBER_SIZE_X / 2)

#define BOX_SIZE_X				(340.0f)
#define BOX_SIZE_Y				(108.0f)
#define BOX_POS_X				(0.0f)
#define BOX_POS_Y				(360.0f - BOX_SIZE_Y / 2)

#define TIMER_START				(0)											// �^�C�}�[�̃X�^�[�g

enum TEXNUM {
	TEX_BOX = 0,
	TEX_TIMER,

	MAX_TEXTURE,
};

enum TIMERROGONUM {
	MIN = 10,
	SEC,
	TIMER,
};

//----- �O���[�o���ϐ���` -----
static int g_nTimer;
static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/numberbox.png",
	L"data/texture/number001.png",
};
static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];

									
//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT InitTimer() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}


	// �ϐ�������
	g_nTimer = TIMER_START;

	return hr;
}


//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitTimer() {
	// �e�N�X�`���J��
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}

}


//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateTimer() {
	//�J�E���g�A�b�v
	g_nTimer++;
}

//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawTimer() {
	int min;
	int sec;
	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetBlendState(BS_ALPHABLEND);

	SetPolygonSize(BOX_SIZE_X, BOX_SIZE_Y);
	SetPolygonPos(BOX_POS_X, BOX_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BOX]);
	DrawPolygon(pDC);

	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonPos(TIMERROGO_POS_X, TIMERROGO_POS_Y);
	SetPolygonUV((TIMER % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
				(TIMER / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
	SetPolygonTexture(g_pTexture[TEX_TIMER]);
	DrawPolygon(pDC);

	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonPos(MINCHR_POS_X, TIMERROGO_POS_Y);
	SetPolygonUV((MIN % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
				(MIN / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
	SetPolygonTexture(g_pTexture[TEX_TIMER]);
	DrawPolygon(pDC);

	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonPos(SECCHR_POS_X, TIMERROGO_POS_Y);
	SetPolygonUV((SEC % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
				(SEC / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
	SetPolygonTexture(g_pTexture[TEX_TIMER]);
	DrawPolygon(pDC);

	
	sec = g_nTimer / 60;
	min = sec / 60;
	if (sec > 59) {
		sec -= 60 * min;
	}

	DrawNumber(XMFLOAT2(MINNUM_POS_X, TIMER_POS_Y),(unsigned)min, MIN_CHRCNT);
	DrawNumber(XMFLOAT2(SECNUM_POS_X, TIMER_POS_Y),(unsigned)sec, SEC_CHRCNT);
	

	// ���ɖ߂�
	SetPolygonColor(1.0f,1.0f,1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetBlendState(BS_NONE);

}


//------------------------------------------------------------------------------------
//
//				�^�C���擾
//
//------------------------------------------------------------------------------------
int GetTimer(){
	return g_nTimer / 60;
}


//------------------------------------------------------------------------------------
//
//				���Z�b�g
//
//------------------------------------------------------------------------------------
void resetTimer() {
	g_nTimer = 0;
}