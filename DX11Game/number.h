//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

// ���l�\��
#pragma once

#include "main.h"

//�萔��`
#define NUMBER_SIZE_X	(1500.0f / 2 / 10)	// ������
#define NUMBER_SIZE_Y	(190.0f / 2)			// ��������

#define NUMBER_COUNT_X	10

//�֐��v���g�^�C�v
HRESULT InitNumber();
void UninitNumber();
//void UpdateNumber();	����͒�`���Ȃ�
void DrawNumber(XMFLOAT2 vPos,					// �\���ʒu�i����j
				unsigned nNumber,				// �\�����l
				int nWidth,						// �\������
				float fSizeX = NUMBER_SIZE_X,	// ������
				float fSizeY = NUMBER_SIZE_Y);	// ��������