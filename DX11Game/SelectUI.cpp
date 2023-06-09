//************************************************************************************
// 
// ���[�h�I����UI[SelectUI.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "SelectUI.h"

#include "Texture.h"
#include "input.h"
#include "Sound.h"

//-------------------- �萔 --------------------
#define PATH_UI_TEXTURE		(L"data/texture/UI_Select.png")
#define PATH_BOX_TEXTURE	(L"data/texture/UI_SelectBox.png")

#define UI_WIDTH			(400)
#define UI_HEIGHT			(240)
#define UI_SPLIT_Y			(2)

#define BOX_WIDTH			(445)
#define BOX_HEIGHT			(165)

std::unique_ptr<Texture> SelectUI::pUI;
std::unique_ptr<Texture> SelectUI::pBox;


//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
SelectUI::SelectUI() {

}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
SelectUI::~SelectUI() {

}


//====================================================================================
//
//				������
//
//====================================================================================
HRESULT SelectUI::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//----- �e�N�X�`���ǂݍ��� -----
	pUI = std::make_unique<Texture>();
	hr = pUI->SetTexture(pDevice, PATH_UI_TEXTURE);

	pBox = std::make_unique<Texture>();
	hr = pBox->SetTexture(pDevice, PATH_BOX_TEXTURE);

	selectNum = (int)START;

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void SelectUI::Uninit() {
	// �e�N�X�`���J��
	pBox->ReleaseTexture();
	pBox.reset();

	pUI->ReleaseTexture();
	pUI.reset();
}


//====================================================================================
//
//				�X�V
//
//====================================================================================
int SelectUI::Update() {
	//----- �I���Ɣ͈͊O���� -----
	if (GetKeyRelease(VK_W) || GetKeyRelease(VK_UP)) {
		selectNum--;
		if (selectNum < START) {
			selectNum = START;
		}
		// SE�Đ�
		CSound::Play(SE_SELECT);
	}
	if (GetKeyRelease(VK_S) || GetKeyRelease(VK_DOWN)) {
		selectNum++;
		if (selectNum > TUTORIAL) {
			selectNum = TUTORIAL;
		}
		// SE�Đ�
		CSound::Play(SE_SELECT);
	}

	// �I�����ꂽ������Ԃ����ƂŌ���L�[�������ꂽ�Ƃ��ɂ��̃V�[���֔��
	return selectNum;
}


//====================================================================================
//
//			�`��
//
//====================================================================================
void SelectUI::Draw() {
	SetBlendState(BS_ALPHABLEND);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	//----- BOX -----
	SetPolygonSize(BOX_WIDTH, BOX_HEIGHT);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(pBox->GetTexture());
	SetPolygonPos(0.0f, 0.0f + (UI_HEIGHT / 2) - (UI_HEIGHT / 2 / 2 + 85)*selectNum);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);


	//----- UI -----
	SetPolygonSize(UI_WIDTH, UI_HEIGHT/2);
	SetPolygonFrameSize(1.0f, 1.0f / UI_SPLIT_Y);

	SetPolygonTexture(pUI->GetTexture());
	for (int i = 0; i < UI_SPLIT_Y; i++) {
		SetPolygonPos(0.0f, 0.0f + (UI_HEIGHT/2) - (UI_HEIGHT/2/2+85)*i);
		SetPolygonUV(0.0f, i / (float)UI_SPLIT_Y);
		DrawPolygon(pDC);
	}



	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}