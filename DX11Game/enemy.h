//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#pragma once

#include "main.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// �G�ő�(�f�o�b�O�p)
#else
#define MAX_ENEMY			(1)	// �G�ő�(�ق񂿂��)
#endif

class GameObject;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
int CollisionEnemy(XMFLOAT3 pos, float radius, float damage);
void CollisionEnemy(GameObject collision);
int GetEnemyKillSum();
void SetRootIndex(XMINT2 *index);
XMINT2 SetStartIndex(int enemyNo);
