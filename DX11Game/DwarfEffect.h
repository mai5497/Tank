//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#pragma once

//-------------------- インクルード --------------------
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDwarfEffect(void);
void UninitDwarfEffect(void);
void UpdateDwarfEffect(void);
void DrawDwarfEffect(void);

int StartDwarfEffect(XMFLOAT3 pos, XMFLOAT2 size = XMFLOAT2(100.0f,100.0f));
void SetDwarfColor(int nDwarf, XMFLOAT4 color);
