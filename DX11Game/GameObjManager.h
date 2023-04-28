//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __GAMEOBJMANAGER_H__
#define __GAMEOBJMANAGER_H__

//-------------------- インクルード --------------------
#include "main.h"

//-------------------- 前方定義 --------------------
class GameObject;
class Collision;
class Game;

//-------------------- クラス定義 --------------------
class GameObjManager {
public:
	//---関数
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
	//---変数
	static int enemySum;											// 敵の総数
	static std::vector<std::shared_ptr<GameObject>> pGameObjects;	// オブジェクトたち
	std::shared_ptr<Collision> pCollManager;						// 当たり判定管理

	static int updateIndex;											// 更新するオブジェクトの要素番号

	Game* pGameScene;												// ゲームシーンを保存
};



#endif