//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

// 数値表示
#pragma once

#include "main.h"

//定数定義
#define NUMBER_SIZE_X	(1500.0f / 2 / 10)	// 文字幅
#define NUMBER_SIZE_Y	(190.0f / 2)			// 文字高さ

#define NUMBER_COUNT_X	10

//関数プロトタイプ
HRESULT InitNumber();
void UninitNumber();
//void UpdateNumber();	今回は定義しない
void DrawNumber(XMFLOAT2 vPos,					// 表示位置（左上）
				unsigned nNumber,				// 表示数値
				int nWidth,						// 表示桁数
				float fSizeX = NUMBER_SIZE_X,	// 文字幅
				float fSizeY = NUMBER_SIZE_Y);	// 文字高さ