//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "main.h"
#include "AssimpModel.h"

class GameObject {
public:
	//---�֐�
	GameObject();
	~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();



	//---�ϐ�
	XMFLOAT3 pos;
	XMFLOAT3 size;
	XMFLOAT3 moveVal;

	XMINT2 mapIndex;	// 0,1�̃}�b�v��������v�f�ԍ������ɂȂ�̂����i�[

	float collRadius;
	XMFLOAT3 collSize;

	XMFLOAT4X4 mtxWorld;
	XMFLOAT3   rotModel;		// ���݂̌���
	/*
	collisiontype
	*/
private:

	CAssimpModel model;

};


#endif