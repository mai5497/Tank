//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "AssimpModel.h"
#include "collision.h"

//-------------------- クラス定義 --------------------
class GameObject {
public:
	//---列挙定数
	enum ObjTag {	// オブジェクトの種類
		NONE = 0,
		PLAYER,
		ENEMY,
		WALL,
		BULLET_PLAYER,
		BULLET_ENEMY

	};

	//---関数
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//---変数
	XMFLOAT3 pos;						// 座標
	XMFLOAT3 size;						// 描画大きさ
	XMFLOAT3 moveVal;					// 移動量

	XMINT2 mapIndex;					// 0,1のマップだったら要素番号いくつになるのかを格納

	Collision::eCollisionType collType;	// オブジェクトが静的か動的か
	float collRadius;					// スフィアコライダーの半径
	XMFLOAT3 collSize;					// ボックスコライダーのxyzの大きさ(直径)

	XMFLOAT4X4 mtxWorld;				// ワールドマトリックス
	XMFLOAT3   rotModel;				// 現在の向き
	
	bool isCollision;					// 当たり判定のオンオフをつける
	bool isHit;							// 何かに当たっている

	ObjTag myTag;						// このオブジェクトのタグ
	std::vector<GameObject> hitList;	// このオブジェクトに当たっているオブジェクト

	bool use;							// 使用フラグ

	int gameObjNum;						// GameObjManagerにあるgameobjのりすとの何番目に格納されているか
	
protected:
	int	shadowNum;						// 丸影番号

};


#endif