#ifndef __WALL_OBJECT_H__
#define __WALL_OBJECT_H__

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWallObj(void);
void UninitWallObj(void);
void UpdateWallObj(void);
void DrawWallObj(void);

XMFLOAT3& GetWallObjPos();
void CollisionWallObj();


#endif