//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "main.h"
#include "Scene.h"


class Game : public Scene {
public:
	Game();
	~Game();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:
	//std::unique_ptr<Player> pPlayer;		// プレイヤーインスタンス
	std::shared_ptr<EnemyManager> pEnemys;	// 敵管理クラス
	std::unique_ptr<MeshField> pMeshField;	// フィールド
	std::unique_ptr<BG> pBG;				// 背景
};

#endif
