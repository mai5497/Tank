//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __SHADOW_H__
#define __SHADOW_H__

//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- プロトタイプ宣言 --------------------
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int CreateShadow(XMFLOAT3 pos, float radius);
void MoveShadow(int nShadow, XMFLOAT3 pos);
void ReleaseShadow(int nShadow);

#endif // !__SHADOW_H__