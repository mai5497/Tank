//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"

//-------------------- クラス定義 --------------------
class Enemy : public GameObject {
public:
	Enemy();
	~Enemy();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	//int CollisionEnemy(XMFLOAT3 pos, float radius, float damage);
	//void CollisionEnemy(GameObject collision);
	void Destroy();
	void SetRootIndex(XMINT2 index);
	XMINT2 SetStartIndex();
	bool GetUse();
private:
	static std::unique_ptr<CAssimpModel> pMyModel;

	XMFLOAT3 rotDest;	// 目的の向き
	int	bulletTimer;	// 弾発射タイマー
	int	rootTimer;		// ルート検索タイマー
	int rootIndexNum;	// 検索したルートの何番目かに使う
	XMINT2* rootIndex;	// 検索したルートが格納される
};

#endif