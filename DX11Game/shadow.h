//=============================================================================
//
// �ۉe [shadow.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int CreateShadow(XMFLOAT3 pos, float radius);
void MoveShadow(int nShadow, XMFLOAT3 pos);
void ReleaseShadow(int nShadow);
