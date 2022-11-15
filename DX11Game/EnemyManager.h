#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "main.h"
#include "enemy.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// 敵最大(デバッグ用)
#else
#define MAX_ENEMY			(1)	// 敵最大(ほんちゃん)
#endif

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	int GetEnemyKillSum();
private:

	int	enemyKillSum;		// 撃破数

	std::shared_ptr<Enemy[]> pEnemys;

};
 
#endif // __ENEMYMANAGER_H__