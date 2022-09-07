// 衝突判定 [collision.h]

#ifndef __COLLISION_H__
#define __COLLISION_H__

//-------------------- インクルード部 --------------------
#include "main.h"
//#include "GameObject.h"

//-------------------- 前方定義 --------------------
class GameObject;

//-------------------- プロトタイプ宣言 --------------------
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
bool CollisionSphere(GameObject A, GameObject B);
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);

GameObject* Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move,XMFLOAT3 Bpos, XMFLOAT3 Bsize);


#endif