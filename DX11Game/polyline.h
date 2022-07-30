//=============================================================================
//
// �|�����C������ [polyline.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include <vector>
#include "main.h"

// �}�N����`
#define MAX_POLYLINE_VERTEX		1024			// �ő咸�_��

// �\���̒�`
struct TPolyline {
	ID3D11Buffer* pVertexBuffer;				// ���_�o�b�t�@ �C���^�[�t�F�[�X�ւ̃|�C���^
	int nNumVertex;								// �����_��

	DirectX::XMFLOAT4X4 mtxWorld;				// ���[���h�}�g���b�N�X
	DirectX::XMFLOAT3 pos;						// �|���S���\���ʒu�̒��S���W
	DirectX::XMFLOAT3 rot;						// �|���S���̉�]�p

	DirectX::XMFLOAT4X4 mtxTexture;				// �e�N�X�`�� �}�g���b�N�X
	ID3D11ShaderResourceView* pTexture;			// �e�N�X�`��

	std::vector<DirectX::XMFLOAT3> pointList;	// ���W���X�g
	float fWidth;								// ����
	bool bRound;								// �p��
	DirectX::XMFLOAT4 cDiffuse;					// �g�U���ːF
	EBlendState bs;								// ����������

	VERTEX_3D	vertex[MAX_POLYLINE_VERTEX];	// ���_�z��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPolyline(void);
void UninitPolyline(void);
void UpdatePolyline(TPolyline* pPolyline);
void DrawPolyline(TPolyline* pPolyline);

HRESULT CreatePolyline(TPolyline* pPolyline, float fWidth = 1.0f, bool bRound = true,
	DirectX::XMFLOAT4 vColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), EBlendState bs = BS_ALPHABLEND);
void ReleasePolyline(TPolyline* pPolyline);
void ClearPolylinePoint(TPolyline* pPolyline);
void AddPolylinePoint(TPolyline* pPolyline, DirectX::XMFLOAT3 pos);
