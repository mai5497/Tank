#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "main.h"

class Scene;
class Fade;

enum eSCENE {
	SCENE_NONE = 0,		// ���ݒ�
	SCENE_TITLE,		// �^�C�g�����
	SCENE_MODESELECT,	// ���[�h�I�����
	SCENE_TUTORIAL,	// �V�ѕ� 
	SCENE_GAME,			// �Q�[�����
	SCENE_RESULT,		// ���ʔ��\
	//SCENE_RANKING,	// �����L���O	

	MAX_SCENE		// �V�[������
};

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetScene(eSCENE eScene);	// �؂�ւ�
	eSCENE GetScene();					// �V�[���ԍ��Ԃ�

private:
	void ChangeScene();

	static eSCENE nextScene;
	eSCENE nowScene;

	std::shared_ptr<Fade> pFade;

	std::shared_ptr<Scene> pNowScene;
};


#endif