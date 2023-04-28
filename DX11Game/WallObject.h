//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

//-------------------- インクルード部 --------------------
#include "GameObject.h"

//-------------------- 定数定義 --------------------
#define MAX_STAGE			(10)
#define MAPWIDTH			(16)		// 壁を置ける横の最大数
#define MAPHEIGHT			(12)		// 壁を置ける縦最大数

//-------------------- クラス定義 --------------------
class  WallObj : public GameObject {
public:
	//---関数
	WallObj();
	WallObj(int mapIndex_x,int mapindex_y);
	~WallObj();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//---変数
	static int wallMap[MAX_STAGE][MAPHEIGHT][MAPWIDTH];
private:
	//---変数
	static std::unique_ptr<CAssimpModel> pMyModel;	// モデル
};

#endif