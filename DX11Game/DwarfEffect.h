//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#pragma once

//-------------------- �C���N���[�h --------------------
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDwarfEffect(void);
void UninitDwarfEffect(void);
void UpdateDwarfEffect(void);
void DrawDwarfEffect(void);

int StartDwarfEffect(XMFLOAT3 pos, XMFLOAT2 size = XMFLOAT2(100.0f,100.0f));
void SetDwarfColor(int nDwarf, XMFLOAT4 color);
