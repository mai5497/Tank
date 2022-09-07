//=============================================================================
//
// ���@���� [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

class GameObject;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

XMFLOAT3& GetPlayerPos();
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
bool CollisionPlayer(GameObject collision);
