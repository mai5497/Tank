//�t�F�[�h�C���E�t�F�[�h�A�E�g
#pragma once

#include "Scene.h"

enum EFade{
	FADE_NONE = 0,		// �������Ă��Ȃ�
	FADE_IN,			// �t�F�[�h�C��������
	FADE_OUT,			// �t�F�[�h�A�E�g������

	MAX_FADE			// �t�F�[�h�ő�l
};


// �v���g�^�C�v�錾
HRESULT InitFade();									// ������
void UninitFade();									// �I��
void UpdateFade();									// �X�V
void DrawFade();									// �`��
//enum EScene;
void StartFadeOut(EScene eNext);						// �t�F�[�h�A�E�g�J�n

EFade GetFade();									// ��Ԏ擾
void SetFadeColor(float fR, float fG, float fB);	// �t�F�[�h�A�E�g�F�ݒ�
