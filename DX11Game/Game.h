//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"
#include "Scene.h"

class GameObjManager;
class MeshField;
class BG;

class Game : public Scene {
public:
	Game();
	~Game();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:
	std::unique_ptr<GameObjManager> pGameObjects;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X
	std::unique_ptr<MeshField> pMeshField;			// �t�B�[���h
	std::unique_ptr<BG> pBG;						// �w�i
};

#endif
