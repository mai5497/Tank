//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __NUMBER_H__
#define __NUMBER_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �萔��` --------------------
#define NUMBER_SIZE_X	(1500.0f / 2 / 10)	// ������
#define NUMBER_SIZE_Y	(190.0f / 2)			// ��������

#define NUMBER_COUNT_X	10

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitNumber();
void UninitNumber();
void DrawNumber(XMFLOAT2 vPos,					// �\���ʒu�i����j
				unsigned nNumber,				// �\�����l
				int nWidth,						// �\������
				float fSizeX = NUMBER_SIZE_X,	// ������
				float fSizeY = NUMBER_SIZE_Y);	// ��������

#endif // !__NUMBER_H__