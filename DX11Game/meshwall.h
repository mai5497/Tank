//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//**************************************
// �`�敔��
//**************************************
enum EDrawPart {
	DRAWPART_ALL = 0,		// ��ʂȂ��\��
	DRAWPART_OPAQUE,		// �s�����̂ݕ`��
	DRAWPART_TRANSLUCENT,	// �������̂ݕ`��

	DRAWPART_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(void);
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(EDrawPart dp = DRAWPART_ALL);
