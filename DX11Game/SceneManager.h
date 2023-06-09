//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �O����` --------------------
class Scene;
class Fade;

//-------------------- �񋓒萔 --------------------
enum eSCENE {
	SCENE_NONE = 0,		// ���ݒ�
	SCENE_TITLE,		// �^�C�g�����
	SCENE_MODESELECT,	// ���[�h�I�����
	SCENE_TUTORIAL,		// �V�ѕ� 
	SCENE_STAGESUMMARY,	// �X�e�[�W�̐��� 
	SCENE_GAME,			// �Q�[�����
	SCENE_RESULT,		// ���ʔ��\

	MAX_SCENE		// �V�[������
};

//-------------------- �N���X��` --------------------
class SceneManager {
public:
	//---�֐�
	SceneManager();
	~SceneManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetScene(eSCENE eScene);	// �؂�ւ�
	eSCENE GetScene();					// �V�[���ԍ��Ԃ�

private:
	//---�֐�
	void ChangeScene();

	//---�ϐ�
	static eSCENE nextScene;			// ���̃V�[���̃X�e�[�g
	eSCENE nowScene;					// ���̃V�[���̃X�e�[�g
	int stageNum;						// �X�e�[�W�̔ԍ�

	std::shared_ptr<Fade> pFade;		// �t�F�[�h

	std::shared_ptr<Scene> pNowScene;	// ���̃V�[��
};


#endif