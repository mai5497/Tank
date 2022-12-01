//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "main.h"
#include "AssimpModel.h"
#include "collision.h"

class GameObject {
public:
	enum ObjTag {
		NONE = 0,
		PLAYER,
		ENEMY,
		WALL,
		BULLET

	};

	//---関数
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//---変数
	XMFLOAT3 pos;
	XMFLOAT3 size;
	XMFLOAT3 moveVal;

	XMINT2 mapIndex;	// 0,1のマップだったら要素番号いくつになるのかを格納

	Collision::eCollisionType collType;	// オブジェクトが静的か動的か
	float collRadius;
	XMFLOAT3 collSize;

	XMFLOAT4X4 mtxWorld;
	XMFLOAT3   rotModel;		// 現在の向き
	
	bool isCollision;	// 当たり判定のオンオフをつける
	bool isHit;			// 何かに当たっている

	ObjTag myTag;		// このオブジェクトのタグ
	std::vector<ObjTag> hitList;	// このオブジェクトに当たっているオブジェクトのタグを取得

	bool use;

	int gameObjNum;	// GameObjManagerにあるgameobjのりすとの何番目に格納されているか
	
protected:
	int	shadowNum;		// 丸影番号

};


#endif