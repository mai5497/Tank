//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"
#include "Scene.h"

class GameObjManager;
class MeshField;
class HPUI;

class Game : public Scene {
public:
	Game(int _stageNum);
	~Game();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	int GetStageNum();

	void StoragePlayerHP(int _hp);
	int GetPlayerHP();

	void StorageEnemySum(int _enemySum);

	void StoragePlayerPos(XMFLOAT3 _pos);
	XMFLOAT3 GetPlayerPos();
private:
	std::unique_ptr<GameObjManager> pGameObjects;	// ゲームオブジェクト管理クラス
	std::unique_ptr<MeshField> pMeshField;			// フィールド

	std::unique_ptr<HPUI> pHPUI;					// HPのUI

	int stageNum;	// 現在のステージ番号

	int playerHP;		// プレイヤーのHPを保存しておく
	int enemySum;		// 敵の残数を保存する
	XMFLOAT3 playerPos;	// プレイヤーの座標を保存しておく
};

#endif