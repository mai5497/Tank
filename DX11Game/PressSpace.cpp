#include "PressSpace.h"

#define TEXTURE_PATH	L"data/texture/pressspace.png"
#define TEXTURE_WIDTH	(1280)
#define TEXTURE_HEIGHT	(195)
#define TEXTURE_POSX	(0.0f)
#define TEXTURE_POSY	(-200.0f)

ID3D11ShaderResourceView* PressSpace::pTexture;
LPCWSTR PressSpace::pszTexFName;

PressSpace::PressSpace() {

}

PressSpace::~PressSpace() {

}

void PressSpace::Init() {
	pszTexFName = TEXTURE_PATH;

	ID3D11Device* pDevice = GetDevice();
	CreateTextureFromFile(pDevice, pszTexFName, &pTexture);

}

void PressSpace::Uninit() {
	SAFE_RELEASE(pTexture);
}

void PressSpace::Update() {

}

void PressSpace::Draw() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(TEXTURE_WIDTH, TEXTURE_HEIGHT);
	SetPolygonPos(TEXTURE_POSX, TEXTURE_POSY);
	SetPolygonTexture(pTexture);
	SetPolygonUV(0.0f, 0.0f);

	DrawPolygon(pDC);

	// Zバッファ無効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);

}