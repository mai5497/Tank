//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "main.h"
#include "GameObject.h"

#define MAPWIDTH			(16)		// �ǂ�u����c�����ʂ̍ő吔
#define MAPHEIGHT			(12)		// �ǂ�u����c�����ʂ̍ő吔

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