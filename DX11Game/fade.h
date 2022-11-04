//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//�t�F�[�h�C���E�t�F�[�h�A�E�g
#pragma once

#include "Scene.h"


enum EFade {
	FADE_NONE = 0,		// �������Ă��Ȃ�
	FADE_IN,			// �t�F�[�h�C��������
	FADE_OUT,			// �t�F�[�h�A�E�g������

	MAX_FADE			// �t�F�[�h�ő�l
};


class Fade {
public:
	Fade();
	~Fade();

	void Init();									// ������
	void Uninit();									// �I��
	void Update();									// �X�V
	void Draw();									// �`��

	void StartFadeOut(std::shared_ptr<Scene> scene);						// �t�F�[�h�A�E�g�J�n

	EFade GetFade();									// ��Ԏ擾
	void SetFadeColor(float fR, float fG, float fB);	// �t�F�[�h�A�E�g�F�ݒ�

private:
	std::shared_ptr<Scene> pNowScene;
};