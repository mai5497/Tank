//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __GAMEOBJMANAGER_H__
#define __GAMEOBJMANAGER_H__

//-------------------- �C���N���[�h --------------------
#include "main.h"

//-------------------- �O����` --------------------
class GameObject;
class Collision;
class Game;

//-------------------- �N���X��` --------------------
class GameObjManager {
public:
	//---�֐�
	GameObjManager();
	~GameObjManager();

	void Init(Game* _pGameScene);
	void Uninit();
	void Update();
	void Draw();

	static int AddList(const std::shared_ptr<GameObject>& _pGameobj, bool _isInit = true);	// ��������Init�֐����ĂԂ��Ă΂Ȃ���
	static void DelList(int _index, bool _isUninit = true);	// ��������Uninit�֐����ĂԂ��Ă΂Ȃ���

	int GetEnemySum();

private:
	//---�ϐ�
	static int enemySum;											// �G�̑���
	static std::vector<std::shared_ptr<GameObject>> pGameObjects;	// �I�u�W�F�N�g����
	std::shared_ptr<Collision> pCollManager;						// �����蔻��Ǘ�

	static int updateIndex;											// �X�V����I�u�W�F�N�g�̗v�f�ԍ�

	Game* pGameScene;												// �Q�[���V�[����ۑ�
};



#endif