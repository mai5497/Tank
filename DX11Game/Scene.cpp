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
Scene::eSCENE Scene::nowScene = Scene::SCENE_NONE;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
Scene::Scene() {
	Init();
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
Scene::~Scene() {
	Uninit();
}


//====================================================================================
//
//				������
//
//====================================================================================
void Scene::Init() {
	HRESULT hr = S_OK;

	// �ŏ��̓^�C�g�����
	oldScene = nowScene = SCENE_TITLE;
	SetScene(nowScene);
	pNowScene = std::make_shared<Title>();

	//pTitle = std::make_shared<Title>();
	//pGame = std::make_shared<Game>();
	//pResult = std::make_shared<Result>();



	pFade = std::make_shared<Fade>();
}


//====================================================================================
//
//				�I��
//
//====================================================================================
void Scene::Uninit() {
	SetScene(SCENE_NONE);	// ���݂̉�ʂ��I��

	//pResult.reset();
	//pGame.reset();
	//pTitle.reset();

	pNowScene.reset();

	pFade.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void Scene::Update() {
	//switch (nowScene) {
	//case SCENE_TITLE:	// �^�C�g�����
	//	pTitle->Update();
	//	break;
	//	//case SCENE_MODE:	// ���[�h�I��
	//	//	UpdateMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	//	UpdateHowToPlay();
	//	//	break;
	//case SCENE_GAME:	// �Q�[�����
	//	pGame->Update();
	//	break;
	//case SCENE_RESULT:	// ���U���g
	//	pResult->Update();
	//	break;
	//	//case SCENE_RANKING:	// �����L���O
	//	//	UpdateRanking();
	//	//	break;
	//default:
	//	break;
	//}
	if (oldScene != nowScene) {
		ChangeScene();
	}

	pNowScene->Update();

	pFade->Update();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void Scene::Draw() {
	//switch (nowScene) {
	//case SCENE_TITLE:	// �^�C�g�����
	//	pTitle->Draw();
	//	break;
	//	//case SCENE_MODE:	// ���[�h�I��
	//	//	DrawMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	//	DrawHowToPlay();
	//	//	break;
	//case SCENE_GAME:	// �Q�[�����
	//	pGame->Draw();
	//	break;
	//case SCENE_RESULT:
	//	pResult->Draw();
	//	break;
	//	//case SCENE_RANKING:
	//	//	DrawRanking();
	//	//	break;
	//default:
	//	break;
	//}

	pNowScene->Draw();

	pFade->Draw();
}


//====================================================================================
//
//				���̃V�[�����Z�b�g
//
//====================================================================================
void Scene::SetScene(eSCENE eScene) {
	nowScene = eScene;
}

//====================================================================================
//
//				�؂�ւ�
//
//====================================================================================
void Scene::ChangeScene() {
	// ���݂̉�ʂ��I��
	//switch (nowScene) {
	//case SCENE_TITLE:		// �^�C�g�����
	//	pTitle->Uninit();
	//	break;
	//	//case SCENE_MODE:	// ���[�h�I��
	//	//	UninitMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	//	UninitHowToPlay();
	//	//	break;
	//case SCENE_GAME:		// �Q�[�����
	//	pGame->Uninit();
	//	break;
	//case SCENE_RESULT:		// ���U���g
	//	pResult->Uninit();
	//	break;
	//	//case SCENE_RANKING:		// �����L���O
	//	//	UninitRanking();
	//	//	break;
	//default:
	//	break;

	//}
	
	pNowScene->Uninit();

	// ��ʂ����ւ�
	//nowScene = nowScene;
	// ���̉�ʂ�������
	//switch (nowScene) {
	//case SCENE_TITLE:		// �^�C�g�����
	//	pTitle->Init();
	//	break;
	//	//case SCENE_MODE:	// ���[�h�I��
	//	//	InitMode();
	//	//	break;
	//	//case SCENE_HOWTOPLAY:	// �V�ѕ�
	//	//	InitHowToPlay();
	//	//	break;
	//case SCENE_GAME:		// �Q�[�����
	//	pGame->Init();
	//	break;
	//case SCENE_RESULT:
	//	pResult->Init();
	//	break;
	//	//case SCENE_RANKING:
	//	//	InitRanking();
	//	//	break;
	//default:
	//	break;
	//}
	switch (nowScene) {
	case SCENE_TITLE:		// �^�C�g�����
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Title>();
		break;
	case SCENE_GAME:		// �Q�[�����
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Game>();
		break;
	case SCENE_RESULT:
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Result>();
		break;
	default:
		break;
	}
	oldScene = nowScene;
	pNowScene->Init();
}


//====================================================================================
//
//				�V�[���ԍ��擾
//
//====================================================================================
Scene::eSCENE Scene::GetScene() {
	return nowScene;
}
