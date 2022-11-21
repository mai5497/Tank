//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "main.h"
#include "GameObject.h"

#define MAPWIDTH			(16)		// 壁を置ける縦横共通の最大数
#define MAPHEIGHT			(12)		// 壁を置ける縦横共通の最大数

class  WallObj : public GameObject {
public:
	WallObj();
	~WallObj();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//XMFLOAT3& GetWallObjPos();

	//void CollisionWallObj();
	//bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage);
};

#endif