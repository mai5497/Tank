//=============================================================================
//
// ���@���� [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#pragma once

#include "main.h"
#include "GameObject.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class Player : public GameObject {
public:
	Player();
	~Player();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3& GetPlayerPos();
	bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
	bool CollisionPlayer(GameObject collision);
	XMINT2 SetPlayerIndex();

private:
	XMFLOAT3 rotDestModel;	// �ړI�̌���

};