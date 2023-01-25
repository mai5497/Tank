#include "PressSpace.h"

#define TEXTURE_PATH	L"data/texture/pressspace.png"
#define TEXTURE_WIDTH	(1280)
#define TEXTURE_HEIGHT	(195)
#define TEXTURE_POSX	(0.0f)
#define TEXTURE_POSY	(-200.0f)

LPCWSTR PressSpace::pszTexFName;
std::unique_ptr<Texture> PressSpace::pTexture;

PressSpace::PressSpace() {

}

PressSpace::~PressSpace() {

}

void PressSpace::Init() {
	pszTexFName = TEXTURE_PATH;

	ID3D11Device* pDevice = GetDevice();
	pTexture = std::make_unique<Texture>();
	pTexture->SetTexture(pDevice, pszTexFName);

}

void PressSpace::Uninit() {
	// �e�N�X�`���̉��
	pTexture->ReleaseTexture();
	pTexture.reset();
}

void PressSpace::Update() {

}

void PressSpace::Draw() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(TEXTURE_WIDTH, TEXTURE_HEIGHT);
	SetPolygonPos(TEXTURE_POSX, TEXTURE_POSY);
	SetPolygonTexture(pTexture->GetTexture());
	SetPolygonUV(0.0f, 0.0f);

	DrawPolygon(pDC);

	// Z�o�b�t�@����(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);

}