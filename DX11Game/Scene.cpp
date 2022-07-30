//----- �C���N���[�h�� -----
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
//#include "HowToPlay.h"
#include "fade.h"


//----- �O���[�o���ϐ� -----
static EScene g_eScene = SCENE_NONE;

//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT InitScene() {
	HRESULT hr = S_OK;

	SetScene(SCENE_TITLE);	// �ŏ��̓^�C�g�����

	InitFade();

	return hr;
}


//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitScene() {
	SetScene(SCENE_NONE);	// ���݂̉�ʂ��I��
	UninitFade();
}

//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateScene() {
	switch (g_eScene) {
	case SCENE_TITLE:	// �^�C�g�����
		UpdateTitle();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	UpdateMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	UpdateHowToPlay();
	//	break;
	case SCENE_GAME:	// �Q�[�����
		UpdateGame();
		break;
	case SCENE_RESULT:	// ���U���g
		UpdateResult();
		break;
	//case SCENE_RANKING:	// �����L���O
	//	UpdateRanking();
	//	break;
	default:
		break;
	}
	UpdateFade();
}

//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawScene() {
	switch (g_eScene) {
	case SCENE_TITLE:	// �^�C�g�����
		DrawTitle();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	DrawMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	DrawHowToPlay();
	//	break;
	case SCENE_GAME:	// �Q�[�����
		DrawGame();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	//case SCENE_RANKING:
	//	DrawRanking();
	//	break;
	default:
		break;
	}
	DrawFade();
}


//------------------------------------------------------------------------------------
//
//				�؂�ւ�
//
//------------------------------------------------------------------------------------
void SetScene(EScene eScene) {
	// ���݂̉�ʂ��I��
	switch (g_eScene) {
	case SCENE_TITLE:		// �^�C�g�����
		UninitTitle();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	UninitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	UninitHowToPlay();
	//	break;
	case SCENE_GAME:		// �Q�[�����
		UninitGame();
		break;
	case SCENE_RESULT:		// ���U���g
		UninitResult();
		break;
	//case SCENE_RANKING:		// �����L���O
	//	UninitRanking();
	//	break;
	default:
		break;

	}
	// ��ʂ����ւ�
	g_eScene = eScene;
	// ���̉�ʂ�������
	switch (g_eScene) {
	case SCENE_TITLE:		// �^�C�g�����
		InitTitle();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	InitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	InitHowToPlay();
	//	break;
	case SCENE_GAME:		// �Q�[�����
		InitGame();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	//case SCENE_RANKING:
	//	InitRanking();
	//	break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------------
//
//				�V�[���ԍ��擾
//
//------------------------------------------------------------------------------------
int GetScene() {
	return g_eScene;
}
