//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SMOKE_H__
#define __SMOKE_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitSmoke(void);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);

int SetSmoke(XMFLOAT3 pos, XMFLOAT2 size,XMFLOAT4 col = XMFLOAT4(1.0f,1.0f,1.0f,1.0f));

#endif