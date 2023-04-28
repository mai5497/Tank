//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __NUMBER_H__
#define __NUMBER_H__

//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- 定数定義 --------------------
#define NUMBER_SIZE_X	(1500.0f / 2 / 10)	// 文字幅
#define NUMBER_SIZE_Y	(190.0f / 2)			// 文字高さ

#define NUMBER_COUNT_X	10

//-------------------- プロトタイプ宣言 --------------------
HRESULT InitNumber();
void UninitNumber();
void DrawNumber(XMFLOAT2 vPos,					// 表示位置（左上）
				unsigned nNumber,				// 表示数値
				int nWidth,						// 表示桁数
				float fSizeX = NUMBER_SIZE_X,	// 文字幅
				float fSizeY = NUMBER_SIZE_Y);	// 文字高さ

#endif // !__NUMBER_H__