//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SHADOW_H__
#define __SHADOW_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int CreateShadow(XMFLOAT3 pos, float radius);
void MoveShadow(int nShadow, XMFLOAT3 pos);
void ReleaseShadow(int nShadow);

#endif // !__SHADOW_H__