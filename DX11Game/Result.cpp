//************************************************************************************
// 
// ���U���g[Result.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Result.h"
#include "Texture.h"
#include "input.h"
#include "Sound.h"
#include "number.h"
#include "Fade.h"
#include "BG.h"
#include "WallObject.h"


//-------------------- �萔��` --------------------
#define TEX_FRAME			(L"data/texture/UI_Result.png")
#define TEX_ALLCLEAR		(L"data/texture/UI_AllClear.png")
#define TEX_STAGECHAR		(L"data/texture/UI_StageChar.png")

#define FRAME_POS_X			(0.0f)
#define FRAME_POS_Y			(20.0f)
#define FRAME_WIDTH			(620.0f)
#define FRAME_HEIGHT		(365.0f)

#define AC_WIDTH			(480.0f/2)
#define AC_HEIGHT			(75.0f/2)
#define AC_POS_X			(0.0f)
#define AC_POS_Y			(0.0f - AC_HEIGHT)

#define STAGECHAR_WIDTH		(700.0f/2)
#define STAGECHAR_HEIGHT	(190.0f/2)
#define STAGECHAR_POS_X		(0.0f - STAGECHAR_WIDTH/4)
#define STAGECHAR_POS_Y		(0.0f + STAGECHAR_HEIGHT/2)

#define STAGENUM_POS_X		(0.0f + NUMBER_SIZE_X+NUMBER_SIZE_X/2)
#define STAGENUM_POS_Y		(0.0f + STAGECHAR_HEIGHT)


std::unique_ptr<Texture> Result::pFrame;
std::unique_ptr<Texture> Result::pAllClear;
std::unique_ptr<Texture> Result::pStageChar;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Result::Result(int _stageNum) {
	stageNum = _stageNum;
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Result::~Result() {
}

//====================================================================================
//
//				������
//
//====================================================================================
void Result::Init() {
	ID3D11Device* pDevice = GetDevice();

	//----- �e�N�X�`���ǂݍ��� -----
	pFrame = std::make_unique<Texture>();
	pFrame->SetTexture(pDevice, TEX_FRAME);

	pAllClear = std::make_unique<Texture>();
	pAllClear->SetTexture(pDevice, TEX_ALLCLEAR);

	pStageChar = std::make_unique<Texture>();
	pStageChar->SetTexture(pDevice, TEX_STAGECHAR);


	// BGM�Đ�
	CSound::Play(BGM_RESULT);
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void Result::Uninit() {

	// BGM�Đ���~
	CSound::Stop(BGM_RESULT);

	// �e�N�X�`���J��
	pStageChar->ReleaseTexture();
	pStageChar.reset();

	pAllClear->ReleaseTexture();
	pAllClear.reset();

	pFrame->ReleaseTexture();
	pFrame.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Result::Update() {
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		// SE�Đ�
		CSound::Play(SE_DECIDE);
		//�Z���N�g��ʂ�
		Fade::StartFadeOut(SCENE_TITLE);
		return;
	}
}

//====================================================================================
//
//				�@�`��
//
//====================================================================================
void Result::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	BG::Draw(SCENE_RESULT);
	
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetBlendState(BS_ALPHABLEND);
	SetZBuffer(false);

	// �g
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonSize(FRAME_WIDTH, FRAME_HEIGHT);
	SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
	SetPolygonUV(1.0f, 1.0f);
	SetPolygonTexture(pFrame->GetTexture());
	DrawPolygon(pDC);


	if (stageNum >= MAX_STAGE) {	// �S�N��
		// �u�X�e�[�W�v�̕���
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(STAGECHAR_WIDTH, STAGECHAR_HEIGHT);
		SetPolygonPos(STAGECHAR_POS_X, STAGECHAR_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pStageChar->GetTexture());
		DrawPolygon(pDC);

		// �X�e�[�W��
		DrawNumber(XMFLOAT2(STAGENUM_POS_X, STAGENUM_POS_Y), (unsigned)stageNum, 2);

		// AllClear
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(AC_WIDTH, AC_HEIGHT);
		SetPolygonPos(AC_POS_X, AC_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pAllClear->GetTexture());
		DrawPolygon(pDC);


	} else {
		// �u�X�e�[�W�v�̕���
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonSize(STAGECHAR_WIDTH, STAGECHAR_HEIGHT);
		SetPolygonPos(STAGECHAR_POS_X, STAGECHAR_POS_Y);
		SetPolygonUV(1.0f, 1.0f);
		SetPolygonTexture(pStageChar->GetTexture());
		DrawPolygon(pDC);

		// �X�e�[�W��
		DrawNumber(XMFLOAT2(STAGENUM_POS_X, STAGENUM_POS_Y), (unsigned)stageNum+1, 2);
	}

	// ���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

	SetZBuffer(true);
	SetBlendState(BS_NONE);
}