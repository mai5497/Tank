//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

//-------------------- �C���N���[�h --------------------
#include "main.h"
#include "Scene.h"

//-------------------- �O����` --------------------
class TitleLogo;
class PressSpace;
class MeshField;
class Particle;
class Texture;

//-------------------- �N���X��` --------------------
class Title : public Scene {
public:
	//---�֐�
	Title();
	~Title();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	//---�ϐ�
	std::unique_ptr<TitleLogo> pLogo;			// �^�C�g�����S
	std::unique_ptr<PressSpace> pStart;			// �v���X�X�y�[�X
	std::unique_ptr<Particle> pParticle;		// �p�[�e�B�N��

	int resetTimer;	// �V�[�������X�^�[�g���鎞��
	int fadeTimer;	// �t�F�[�h�ɓ���܂ł̑ҋ@����
	bool isFade;	// �t�F�[�h�ɓ��鋖�t���O
};

#endif // !__TITLE_H__

