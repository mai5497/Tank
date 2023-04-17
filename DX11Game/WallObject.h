//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "GameObject.h"

#define MAXMAP				(10)		// ステージの最大数
#define MAPWIDTH			(16)		// 壁を置ける横の最大数
#define MAPHEIGHT			(12)		// 壁を置ける縦最大数

class  WallObj : public GameObject {
public:
	WallObj();
	WallObj(int x,int y);
	~WallObj();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//XMFLOAT3& GetWallObjPos();

	//void CollisionWallObj();
	//bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage);
	static int wallMap[MAXMAP][MAPHEIGHT][MAPWIDTH];
private:
	static std::unique_ptr<CAssimpModel> pMyModel;

};

#endif