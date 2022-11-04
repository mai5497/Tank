//=============================================================================
//
// 自機処理 [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#pragma once

#include "main.h"
#include "GameObject.h"

//*****************************************************************************
// プロトタイプ宣言
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
	XMFLOAT3 rotDestModel;	// 目的の向き

};