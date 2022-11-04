//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "main.h"
#include "AssimpModel.h"

class GameObject {
public:
	//---関数
	GameObject();
	~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();



	//---変数
	XMFLOAT3 pos;
	XMFLOAT3 size;
	XMFLOAT3 moveVal;

	XMINT2 mapIndex;	// 0,1のマップだったら要素番号いくつになるのかを格納

	float collRadius;
	XMFLOAT3 collSize;

	XMFLOAT4X4 mtxWorld;
	XMFLOAT3   rotModel;		// 現在の向き
	/*
	collisiontype
	*/
private:

	CAssimpModel model;

};


#endif