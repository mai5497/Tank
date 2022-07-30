//=============================================================================
//
// �G�@���� [enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// �G�ő�(�f�o�b�O�p)
#else
#define MAX_ENEMY			(50)	// �G�ő�(�ق񂿂��)
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
int CollisionEnemy(XMFLOAT3 pos, float radius, float damage);
int GetEnemyKillSum();