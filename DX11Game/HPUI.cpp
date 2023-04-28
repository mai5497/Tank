//************************************************************************************
// 
// HP��UI[HPUI.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "HPUI.h"
#include "Player.h"
#include "Game.h"
#include "debugproc.h"

//-------------------- �萔��` --------------------

#define PATH_EMPTY_TEXTURE	(L"data/texture/UI_EmptyHeart.png")
#define PATH_FULL_TEXTURE	(L"data/texture/UI_FullHeart.png")

#define UI_WIDTH			(145.0f / 2)
#define UI_HEIGHT			(145.0f / 2)

#define UI_BASE_POS_X		(SCREEN_WIDTH/2 - MAX_HP * UI_WIDTH + UI_WIDTH/2)
#define UI_BASE_POS_Y		(SCREEN_HEIGHT/2 - UI_HEIGHT + UI_HEIGHT/2)

std::unique_ptr<Texture> HPUI::pEmpty;
std::unique_ptr<Texture> HPUI::pFull;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
HPUI::HPUI() {

}



//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
HPUI::~HPUI() {

}



//====================================================================================
//
//				������
//
//====================================================================================
void HPUI::Init(Game* _pGameScene) {
	//----- �ϐ������� -----
	pGameScene = _pGameScene;	// �Q�[���V�[����ێ�

	playerHP = MAX_HP;	// HP�̏�����

	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	pEmpty = std::make_unique<Texture>();
	pEmpty->SetTexture(pDevice, PATH_EMPTY_TEXTURE);

	pFull = std::make_unique<Texture>();
	pFull->SetTexture(pDevice, PATH_FULL_TEXTURE);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void HPUI::Uninit() {
	//----- HPUI��� -----
	pFull->ReleaseTexture();
	pFull.reset();

	pEmpty->ReleaseTexture();
	pEmpty.reset();	
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void HPUI::Update() {
	// HP�̎擾
	playerHP = pGameScene->GetPlayerHP();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void HPUI::Draw() {
	SetBlendState(BS_ALPHABLEND);
	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	SetPolygonSize(UI_WIDTH, UI_HEIGHT);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < MAX_HP; i++) {
		if (i < MAX_HP - playerHP) {	// HP�����邩�Ȃ����ǂ�����`�悷�邩�̔���
			SetPolygonTexture(pEmpty->GetTexture());
		} else {
			SetPolygonTexture(pFull->GetTexture());
		}
		
		SetPolygonPos(UI_BASE_POS_X + i * UI_WIDTH, UI_BASE_POS_Y);
		DrawPolygon(pDC);
	}

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);

}
