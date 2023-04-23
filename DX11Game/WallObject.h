//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "GameObject.h"

#define MAX_STAGE				(10)		// �X�e�[�W�̍ő吔
#define MAPWIDTH			(16)		// �ǂ�u���鉡�̍ő吔
#define MAPHEIGHT			(12)		// �ǂ�u����c�ő吔

class  WallObj : public GameObject {
public:
	WallObj();
	WallObj(int mapIndex_x,int mapindex_y);
	~WallObj();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//XMFLOAT3& GetWallObjPos();

	//void CollisionWallObj();
	//bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage);
	static int wallMap[MAX_STAGE][MAPHEIGHT][MAPWIDTH];
private:
	static std::unique_ptr<CAssimpModel> pMyModel;

};

#endif