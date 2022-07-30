// �w�i [bg.cpp]
#include "bg.h"
#include "Texture.h"
#include "polygon.h"

// �}�N����`
#define PATH_BGTEXTURE	L"data/texture/sky001.jpg"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;

// ������
HRESULT InitBG()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);

	return hr;
}

// �I������
void UninitBG()
{
	// �w�i�e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

// �X�V
void UpdateBG()
{
	// �������Ȃ�
}

// �`��
void DrawBG()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);
}
