//----- �C���N���[�h�� -----
#include "Result.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"
#include "number.h"
#include "timer.h"
#include "fade.h"


//----- �萔��` -----
#define RESULT_POS_X		(0.0f)
#define RESULT_POS_Y		(0.0f)
#define RESULT_WIDTH		(SCREEN_WIDTH)
#define RESULT_HEIGHT		(SCREEN_HEIGHT)

#define TIMER_POS_Y			(0.0f + (NUMBER_SIZE_Y*2.5) * 0.5)				// �\���ʒu
#define MIN_CHRCNT			(2)												// ��������
#define SEC_CHRCNT			(2)												// �b������
#define TIMERROGO_POS_Y		(TIMER_POS_Y - (NUMBER_SIZE_Y*2.5) * 0.5)

#define MINCHR_POS_X		(0.0f - (NUMBER_SIZE_X*2.5) * 0.5)															// �������ʒu
#define MINNUM_POS_X		(MINCHR_POS_X - MIN_CHRCNT * (NUMBER_SIZE_X*2.5) - (NUMBER_SIZE_X*2.5) / 2)	// �������ʒu
#define SECNUM_POS_X		(MINCHR_POS_X + (NUMBER_SIZE_X*2.5) - (NUMBER_SIZE_X*2.5) / 2)
#define SECCHR_POS_X		(SECNUM_POS_X + SEC_CHRCNT * (NUMBER_SIZE_X*2.5) + (NUMBER_SIZE_X*2.5) / 2)



enum TEXNUM {
	TEX_BG = 0,
	TEX_TIMER,

	MAX_TEXTURE,
};

enum TIMERROGONUM {
	MIN = 10,
	SEC,
};


//----- �O���[�o���ϐ��錾 -----
static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/result.png",
	L"data/texture/number001.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];

//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT	InitResult() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}

	// BGM�Đ�
	CSound::Play(BGM_RESULT);

	return hr;
}

//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitResult() {

	// BGM�Đ���~
	CSound::Stop(BGM_RESULT);

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
void UpdateResult() {
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		CSound::Play(SE_DECIDE);
		//�����L���O��ʂ�
		//SetScene(SCENE_TITLE);
		StartFadeOut(SCENE_TITLE);
		return;
	}
}

//------------------------------------------------------------------------------------
//
//				�@�`��
//
//------------------------------------------------------------------------------------
void DrawResult() {
	int min;
	int sec;

	ID3D11DeviceContext *pDC = GetDeviceContext();
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);



	SetPolygonSize(RESULT_WIDTH, RESULT_HEIGHT);
	SetPolygonPos(RESULT_POS_X, RESULT_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);

	SetBlendState(BS_ADDITIVE);		// ���Z����
	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
	SetPolygonSize(NUMBER_SIZE_X * 2.5, NUMBER_SIZE_Y * 2.5);
	SetPolygonPos(MINCHR_POS_X, TIMERROGO_POS_Y);
	SetPolygonUV((MIN % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
		(MIN / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
	SetPolygonTexture(g_pTexture[TEX_TIMER]);
	DrawPolygon(pDC);

	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
	SetPolygonSize(NUMBER_SIZE_X * 2.5, NUMBER_SIZE_Y * 2.5);
	SetPolygonPos(SECCHR_POS_X, TIMERROGO_POS_Y);
	SetPolygonUV((SEC % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
		(SEC / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
	SetPolygonTexture(g_pTexture[TEX_TIMER]);
	DrawPolygon(pDC);


	sec = GetTimer();
	min = sec / 60;
	if (sec > 59) {
		sec -= 60 * min;
	}

	DrawNumber(XMFLOAT2(MINNUM_POS_X, TIMER_POS_Y), (unsigned)min, MIN_CHRCNT,NUMBER_SIZE_X * 2.5, NUMBER_SIZE_Y * 2.5);
	DrawNumber(XMFLOAT2(SECNUM_POS_X, TIMER_POS_Y), (unsigned)sec, SEC_CHRCNT,NUMBER_SIZE_X * 2.5, NUMBER_SIZE_Y * 2.5);

	SetZBuffer(true);
	SetBlendState(BS_NONE);


	// ���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

}