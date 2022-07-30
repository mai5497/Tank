//----- �C���N���[�h�� -----
#include "title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "fade.h"
#include "Sound.h"

//----- �萔��` -----
#define TITLE_POS_X			0.0f
#define TITLE_POS_Y			0.0f
#define TITLE_WIDTH			SCREEN_WIDTH
#define TITLE_HEIGHT		SCREEN_HEIGHT

//----- �O���[�o���ϐ��錾 -----
static LPCWSTR g_pszTexFName = {
	L"data/texture/title.png"
};

static ID3D11ShaderResourceView *g_pTexture;

//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT	InitTitle() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	hr = CreateTextureFromFile(pDevice, g_pszTexFName, &g_pTexture);
	if (FAILED(hr)) {
		return hr;
	}


	// BGM�Đ�
	CSound::Play(BGM_TITLE);

	return hr;
}

//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitTitle() {

	// BGM�Đ���~
	CSound::Stop(BGM_TITLE);
	// �w�i�e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
}

//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateTitle() {
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		CSound::Play(SE_DECIDE);
		//���[�h�I����ʂ�
		//SetScene(SCENE_GAME);
		StartFadeOut(SCENE_GAME);
		return;
	}
}

//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawTitle() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);

	DrawPolygon(pDC);
}