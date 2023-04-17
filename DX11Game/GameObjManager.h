#ifndef __GAMEOBJMANAGER_H__
#define __GAMEOBJMANAGER_H__

#include "main.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// �G�ő�(�f�o�b�O�p)
#else
#define MAX_ENEMY			(10)	// �G�ő�(�ق񂿂��)
#endif

class GameObject;
class Collision;
class Game;

class GameObjManager {
public:
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
	static int enemySum;
	static std::vector<std::shared_ptr<GameObject>> pGameObjects;
	std::shared_ptr<Collision> pCollManager;

	static int updateIndex;

	Game* pGameScene;
};



#endif