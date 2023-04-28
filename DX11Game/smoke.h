//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmoke(void);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);

int SetSmoke(XMFLOAT3 pos, XMFLOAT2 size,XMFLOAT4 col = XMFLOAT4(1.0f,1.0f,1.0f,1.0f));
