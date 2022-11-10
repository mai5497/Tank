//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __SCENE_H__
#define __SCENE_H__

#include "main.h"

class Game;
class Title;
class Result;
class Fade;

class Scene {
public:
	enum eSCENE {
		SCENE_NONE = 0,	// ���ݒ�
		SCENE_TITLE,	// �^�C�g�����
		//SCENE_MODE,		// ���[�h�I�����
		//SCENE_HOWTOPLAY,// �V�ѕ� 
		SCENE_GAME,		// �Q�[�����
		SCENE_RESULT,	// ���ʔ��\
		//SCENE_RANKING,	// �����L���O	

		MAX_SCENE		// �V�[������
	};

	Scene();
	~Scene();

	void Init();			// ������
	void Uninit();			// �I������
	void Update();			// �X�V
	void Draw();			// �`��



	static void SetScene(eSCENE eScene);	// �؂�ւ�
	void ChangeScene();
	eSCENE GetScene();					// �V�[���ԍ��Ԃ�

private:

	std::shared_ptr<Fade> pFade;
	static eSCENE nowScene;
	eSCENE oldScene;

protected:

	// �Ƃ肠���������������߂Ƀ|�C���^����Ƃ�
	//std::shared_ptr <Scene> pGame;
	//std::shared_ptr<Title> pTitle;
	//std::shared_ptr<Result> pResult;

	std::shared_ptr<Scene> pNowScene;

};

#endif
