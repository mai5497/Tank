//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

//-------------------- �C���N���[�h --------------------
#include "main.h"
#include "Scene.h"

//-------------------- �O����` --------------------
class Texture;

//-------------------- �N���X��` --------------------
class Result : public Scene {
public:
	//---�֐�
	Result(int _stageNum);
	~Result();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	//---�ϐ�
	static std::unique_ptr<Texture> pFrame;		// UI�g
	static std::unique_ptr<Texture> pAllClear;	// UI�I�[���N���A
	static std::unique_ptr<Texture> pStageChar;	// UI�u�X�e�[�W�v�̕���

	int stageNum;								// �X�e�[�W�̔ԍ�

};
#endif

