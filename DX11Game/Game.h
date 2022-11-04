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

class Game : public Scene {
public:
	Game();
	~Game();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	std::unique_ptr<Player> pPlayer;	// �v���C���[�C���X�^���X

};

#endif
