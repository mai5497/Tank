#ifndef __GAMEOBJMANAGER_H__
#define __GAMEOBJMANAGER_H__

#include "main.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// 敵最大(デバッグ用)
#else
#define MAX_ENEMY			(10)	// 敵最大(ほんちゃん)
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

	static int AddList(const std::shared_ptr<GameObject>& _pGameobj, bool _isInit = true);	// 第二引数はInit関数を呼ぶか呼ばないか
	static void DelList(int _index, bool _isUninit = true);	// 第二引数はUninit関数を呼ぶか呼ばないか

	int GetEnemySum();

private:
	static int enemySum;
	static std::vector<std::shared_ptr<GameObject>> pGameObjects;
	std::shared_ptr<Collision> pCollManager;

	static int updateIndex;

	Game* pGameScene;
};



#endif