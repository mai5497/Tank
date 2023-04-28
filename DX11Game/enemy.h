//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

//-------------------- インクルード --------------------
#include "GameObject.h"

//-------------------- 前方定義 --------------------
class BulletLine;
class Game;

//-------------------- クラス定義 --------------------
class Enemy : public GameObject {
public:
	//---関数
	Enemy(int mapIndex_x, int mapindex_y,Game *_pGameScene);
	~Enemy();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Destroy();
	void SetRootIndex(XMINT2 index);
	XMINT2 SetStartIndex();
	bool GetUse();
private:
	//---変数
	Game* pGameScene;									// ゲームシーンを保持
	
	static std::unique_ptr<CAssimpModel> pMyModel;		// モデル

	XMFLOAT3 rotDest;									// 目的の向き
	int	bulletTimer;									// 弾発射タイマー
	int	rootTimer;										// ルート検索タイマー
	std::vector<XMINT2>::iterator rootIndexNum;			// 検索したルートの何番目かに使う
	std::vector<XMINT2> rootIndex;						// 検索したルートが格納される

	std::unique_ptr<BulletLine> pBulletLine;			// 撃つ方向を描画

	XMFLOAT3 playerPos;									// プレイヤーに照準を向ける為座標を保存
};

#endif