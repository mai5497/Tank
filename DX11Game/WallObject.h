//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "GameObject.h"

#define MAXMAP				(10)		// �X�e�[�W�̍ő吔
#define MAPWIDTH			(16)		// �ǂ�u���鉡�̍ő吔
#define MAPHEIGHT			(12)		// �ǂ�u����c�ő吔

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