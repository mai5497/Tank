//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#pragma once

//-------------------- �C���N���[�h --------------------
#include "main.h"

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);
int SetEffect(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT2 size, int timer);