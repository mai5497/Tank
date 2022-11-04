//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#pragma once

#include "main.h"

#ifdef _DEBUG
#define MAX_ENEMY			(1)		// 敵最大(デバッグ用)
#else
#define MAX_ENEMY			(1)	// 敵最大(ほんちゃん)
#endif

class GameObject;

//*****************************************************************************
// プロトタイプ宣言
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
