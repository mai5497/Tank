#include "Tutorial.h"

#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "Sound.h"

#include "bg.h"


#define PATH_UI_TEXTURE	L"data/texture/UI_Sousa.png"

#define UI_WIDTH		(615)
#define UI_HEIGHT		(401)


// �ÓI�����o�ϐ�������
std::unique_ptr<Texture> Tutorial::pUI;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Tutorial::Tutorial() {

}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Tutorial::~Tutorial() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void Tutorial::Init() {
	ID3D11Device* pDevice = GetDevice();

	pUI = std::make_unique<Texture>();
	pUI->SetTexture(pDevice,PATH_UI_TEXTURE);
	
	// BGM�Đ�
	CSound::Play(BGM_MODE);
}


//====================================================================================
//
//				�I��
//
//====================================================================================
void Tutorial::Uninit() {
	// BGM�Đ���~
	CSound::Stop(BGM_MODE);

	pUI->ReleaseTexture();
	pUI.reset();
}


//====================================================================================
//
//				�X�V
//
//====================================================================================
void Tutorial::Update() {
	if (GetKeyRelease(VK_RETURN)) {
		Fade::StartFadeOut(SCENE_MODESELECT);
	}
}


//====================================================================================
//
//				�`��
//
//====================================================================================
void Tutorial::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	BG::Draw(SCENE_TUTORIAL);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonTexture(pUI->GetTexture());
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}