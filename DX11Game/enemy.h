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

//-------------------- クラス定義 --------------------
class Enemy : public GameObject {
public:
	Enemy(int mapIndex_x, int mapindex_y);
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
	static std::unique_ptr<CAssimpModel> pMyModel;

	XMFLOAT3 rotDest;	// 目的の向き
	int	bulletTimer;	// 弾発射タイマー
	int	rootTimer;		// ルート検索タイマー
	std::vector<XMINT2>::iterator rootIndexNum;	// 検索したルートの何番目かに使う
	std::vector<XMINT2> rootIndex;	// 検索したルートが格納される

	std::unique_ptr<BulletLine> pBulletLine;
};

#endif