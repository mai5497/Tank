//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

//-------------------- �C���N���[�h�� --------------------
#include "GameObject.h"

//-------------------- �萔��` --------------------
#define MAX_STAGE			(10)
#define MAPWIDTH			(16)		// �ǂ�u���鉡�̍ő吔
#define MAPHEIGHT			(12)		// �ǂ�u����c�ő吔

//-------------------- �N���X��` --------------------
class  WallObj : public GameObject {
public:
	//---�֐�
	WallObj();
	WallObj(int mapIndex_x,int mapindex_y);
	~WallObj();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//---�ϐ�
	static int wallMap[MAX_STAGE][MAPHEIGHT][MAPWIDTH];
private:
	//---�ϐ�
	static std::unique_ptr<CAssimpModel> pMyModel;	// ���f��
};

#endif