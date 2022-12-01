//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

// �Փ˔��� [collision.h]

#ifndef __COLLISION_H__
#define __COLLISION_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �O����` --------------------
class GameObject;

//-------------------- �v���g�^�C�v�錾 --------------------
class Collision {
public:
	enum eCollisionType {
		DYNAMIC = 0,
		STATIC,
	};

	Collision();
	~Collision();

	static void AddList(const std::shared_ptr<GameObject>& _gameObject);
	static void DelList(int _index);
	void Update();

	bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
	bool CollisionSphere(GameObject A, GameObject B);
	bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	bool CollisionAABB(GameObject A, GameObject B);

	GameObject* Push(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 move, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	void Push(std::shared_ptr<GameObject> A, std::shared_ptr<GameObject> B);

private:
	static std::vector<std::shared_ptr<GameObject>> pObjects;
	static int updateIndex;
};

#endif