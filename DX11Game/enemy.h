//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "main.h"
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
	void SetRootIndex(XMINT2 index);
	XMINT2 SetStartIndex();
	bool GetUse();
private:
	//XMFLOAT3	pos;	// 現在の位置
	XMFLOAT3 rotDest;	// 目的の向き
	//XMFLOAT3	move;	// 移動量
	//XMFLOAT3	size;	// 大きさ
	bool use;			// 使用中かどうか
	int	bulletTimer;	// 弾発射タイマー
	int	rootTimer;		// ルート検索タイマー
	//XMINT2	mapIndex;	// 座標をマップ番号で表したものyxの順に格納

	//XMFLOAT4X4	m_mtxWorld;		// ワールドマトリックス

	int	shadowNum;		// 丸影番号

};

#endif