#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "main.h"
#include "enemy.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// �G�ő�(�f�o�b�O�p)
#else
#define MAX_ENEMY			(1)	// �G�ő�(�ق񂿂��)
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

	int	enemyKillSum;		// ���j��

	std::shared_ptr<Enemy[]> pEnemys;

};
 
#endif // __ENEMYMANAGER_H__