//=============================================================================
//
// 自機処理 [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

class GameObject;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

XMFLOAT3& GetPlayerPos();
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
bool CollisionPlayer(GameObject collision);
