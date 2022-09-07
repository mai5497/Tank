// �Փ˔��� [collision.h]

#ifndef __COLLISION_H__
#define __COLLISION_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"
//#include "GameObject.h"

//-------------------- �O����` --------------------
class GameObject;

//-------------------- �v���g�^�C�v�錾 --------------------
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
bool CollisionSphere(GameObject A, GameObject B);
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);

GameObject* Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move,XMFLOAT3 Bpos, XMFLOAT3 Bsize);


#endif