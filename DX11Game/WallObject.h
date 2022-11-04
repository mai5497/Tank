//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "main.h"

#define MAPWIDTH			(16)		// 壁を置ける縦横共通の最大数
#define MAPHEIGHT			(12)		// 壁を置ける縦横共通の最大数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWallObj(void);
void UninitWallObj(void);
void UpdateWallObj(void);
void DrawWallObj(void);

//XMFLOAT3& GetWallObjPos();
void CollisionWallObj();
bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage);


#endif