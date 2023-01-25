//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

#include "main.h"
#include "Scene.h"

class TitleLogo;
class PressSpace;
class BG;
class MeshField;
class Particle;
class Texture;

//===========================
//
//	�N���X��`
//
//===========================
class Title : public Scene {
public:
	Title();
	~Title();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	std::unique_ptr<TitleLogo> pLogo;			// �^�C�g�����S
	std::unique_ptr<PressSpace> pStart;			// �v���X�X�y�[�X
	std::unique_ptr<BG> pBG;					// �w�i
	std::unique_ptr<MeshField> pMeshField;		// �t�B�[���h
	std::unique_ptr<Particle> pParticle;		// �p�[�e�B�N��

	int timer;
	int fadeTimer;
	bool isFade;
};

#endif // !__TITLE_H__

