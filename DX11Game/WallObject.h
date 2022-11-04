//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "main.h"

#define MAPWIDTH			(16)		// �ǂ�u����c�����ʂ̍ő吔
#define MAPHEIGHT			(12)		// �ǂ�u����c�����ʂ̍ő吔


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWallObj(void);
void UninitWallObj(void);
void UpdateWallObj(void);
void DrawWallObj(void);

//XMFLOAT3& GetWallObjPos();
void CollisionWallObj();
bool CollisionWalltoBullet(XMFLOAT3 pos, float radius, float damage);


#endif