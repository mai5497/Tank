//************************************************************************************
// 
// �V�[���Ǘ�[SceneManager.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#include "SceneManager.h"
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Fade.h"
#include "Camera.h"
#include "Select.h"

//-------------------- �O���[�o���ϐ���` --------------------
eSCENE SceneManager::nextScene = SCENE_NONE;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
SceneManager::SceneManager() {
}

//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
SceneManager::~SceneManager() {
}

//====================================================================================
//
//				������
//
//====================================================================================
void SceneManager::Init() {
	// �ŏ��̓^�C�g�����
	nowScene = nextScene = SCENE_TITLE;
	if(!pNowScene)pNowScene = std::make_shared<Title>();
	pNowScene->Init();

	// �t�F�[�h�̏�����
	if(!pFade)pFade = std::make_shared<Fade>();
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void SceneManager::Uninit() {
	SetScene(SCENE_NONE);	// ���݂̉�ʂ��I��

	//----- �������̊J�� -----
	if(pNowScene)pNowScene.reset();
	if(pFade)pFade.reset();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void SceneManager::Update() {

	//----- �V�[���؂�ւ� -----
	if (nowScene != nextScene) {
		ChangeScene();
	}


	//----- �e��X�V���� -----
	if (!pNowScene) {
		MessageBox(NULL, _T("�V�[�������G���[\nSceneManager.cpp(78)"), _T("error"), MB_OK);
	}
	//----- �J�����X�V -----
	CCamera::Get()->Update(nowScene);

	pNowScene->Update();

	pFade->Update();
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void SceneManager::Draw() {	
	pNowScene->Draw();
	pFade->Draw();
}

//====================================================================================
//
//				���̃V�[�����Z�b�g
//
//====================================================================================
void SceneManager::SetScene(eSCENE eScene) {
	nextScene = eScene;	// ���̃V�[���Ƃ��Ċi�[���Ă����AUpdate�Ő؂�ւ��̊֐����Ă�
}

//====================================================================================
//
//				�V�[���ԍ��擾
//
//====================================================================================
eSCENE SceneManager::GetScene() {
	return nowScene;
}


//====================================================================================
//
//				�؂�ւ�
//
//====================================================================================
void SceneManager::ChangeScene() {
	// ���݂̃V�[���̏I������
	pNowScene->Uninit();
	
	// �J�����̏�����
	CCamera::Get()->Init();

	// ���̃V�[���ɂ���Ċi�[������̂�������
	switch (nextScene) {
	case SCENE_TITLE:		// �^�C�g�����
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Title>();
		break;
	case SCENE_MODESELECT:	// �Z���N�g���
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Select>();
		break;
	case SCENE_GAME:		// �Q�[�����
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Game>();
		break;
	case SCENE_RESULT:		// ���U���g�V�[��
		pNowScene.reset();	// �������؂�ւ��̈הj��
		pNowScene = std::make_shared<Result>();
		break;
	default:
		MessageBox(NULL, _T("�V�[���̐؂�ւ��Ɏ��s���܂����B\nSceneManager.cpp(148)"), _T("error"), MB_OK);
		break;
	}

	// �V�����������ɂȂ����̂ŏ�����
	pNowScene->Init();

	// ���݂̃V�[���ԍ������������Ă���
	nowScene = nextScene;
}
