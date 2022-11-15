//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "main.h"
#include "Scene.h"
#include "Player.h"
#include "EnemyManager.h"

class Game : public Scene {
public:
	Game();
	~Game();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:
	std::unique_ptr<Player> pPlayer;	// �v���C���[�C���X�^���X
	std::shared_ptr<EnemyManager> pEnemys;
};

#endif
