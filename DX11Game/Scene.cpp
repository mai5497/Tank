//************************************************************************************
// 
// �V�[���Ǘ�[Scene.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
//#include "HowToPlay.h"
#include "fade.h"


//-------------------- �O���[�o���ϐ���` --------------------
static Scene::eSCENE g_eScene = Scene::SCENE_NONE;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Scene::Scene() {

}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Scene::~Scene() {

}


//====================================================================================
//
//				������
//
//====================================================================================
void Scene::Init() {
	HRESULT hr = S_OK;

	SetScene(SCENE_TITLE);	// �ŏ��̓^�C�g�����

	pTitle = new Title;
	pGame = new Game;

	InitFade();
}


//====================================================================================
//
//				�I��
//
//====================================================================================
void Scene::Uninit() {
	SetScene(SCENE_NONE);	// ���݂̉�ʂ��I��

	delete pGame;
	delete pTitle;

	UninitFade();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Scene::Update() {
	switch (g_eScene) {
	case SCENE_TITLE:	// �^�C�g�����
		pTitle->Update();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	UpdateMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	UpdateHowToPlay();
	//	break;
	case SCENE_GAME:	// �Q�[�����
		pGame->Update();
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

//====================================================================================
//
//				�`��
//
//====================================================================================
void Scene::Draw() {
	switch (g_eScene) {
	case SCENE_TITLE:	// �^�C�g�����
		pTitle->Draw();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	DrawMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	DrawHowToPlay();
	//	break;
	case SCENE_GAME:	// �Q�[�����
		pGame->Draw();
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


//====================================================================================
//
//				�؂�ւ�
//
//====================================================================================
void Scene::SetScene(eSCENE eScene) {
	// ���݂̉�ʂ��I��
	switch (g_eScene) {
	case SCENE_TITLE:		// �^�C�g�����
		pTitle->Uninit();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	UninitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	UninitHowToPlay();
	//	break;
	case SCENE_GAME:		// �Q�[�����
		pGame->Uninit();
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
		pTitle->Init();
		break;
	//case SCENE_MODE:	// ���[�h�I��
	//	InitMode();
	//	break;
	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	InitHowToPlay();
	//	break;
	case SCENE_GAME:		// �Q�[�����
		pGame->Init();
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

//====================================================================================
//
//				�V�[���ԍ��擾
//
//====================================================================================
Scene::eSCENE Scene::GetScene() {
	return g_eScene;
}
