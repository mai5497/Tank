//************************************************************************************
// 
// �w�i[bg.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "BG.h"
#include "Texture.h"

//-------------------- �萔��` --------------------
#define PATH_TITLE_TEXTURE	L"data/texture/BG_Sky.jpg"
#define PATH_SELECT_TEXTURE	L"data/texture/BG_Select.png"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

std::unique_ptr<Texture> BG::pSkyBG;
std::unique_ptr<Texture> BG::pSelectBG;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
BG::BG() {

}



//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
BG::~BG() {

}



//====================================================================================
//
//				������
//
//====================================================================================
HRESULT BG::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	pSkyBG = std::make_unique<Texture>();
	hr = pSkyBG->SetTexture(pDevice, PATH_TITLE_TEXTURE);

	pSelectBG = std::make_unique<Texture>();
	hr = pSelectBG->SetTexture(pDevice, PATH_SELECT_TEXTURE);

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void BG::Uninit() {
	// �w�i�e�N�X�`�����
	pSelectBG->ReleaseTexture();
	pSelectBG.reset();

	pSkyBG->ReleaseTexture();
	pSkyBG.reset();

}

//====================================================================================
//
//				�`��
//
//====================================================================================
void BG::Draw(eSCENE _nowScene) {
	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	if (_nowScene == SCENE_TITLE || _nowScene == SCENE_GAME) {
		SetPolygonTexture(pSkyBG->GetTexture());
	} else if (_nowScene == SCENE_MODESELECT || _nowScene == SCENE_STAGESUMMARY || _nowScene == SCENE_TUTORIAL || _nowScene == SCENE_RESULT) {
		SetPolygonTexture(pSelectBG->GetTexture());
	}
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);


	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
}
