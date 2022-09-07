#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "main.h"

class GameObject {
public:
	//---ä÷êî
	GameObject();
	~GameObject();


	//---ïœêî
	XMFLOAT3 m_pos;
	XMFLOAT3 m_size;
	XMFLOAT3 m_move;

	float m_radius;

	XMFLOAT4X4 m_mtxWorld;

	/*
	collisiontype
	radius
	*/



private:

};


#endif