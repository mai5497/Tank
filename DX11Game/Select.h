//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __SELECT_H__
#define __SELECT_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"
#include "Scene.h"

//-------------------- �O����` --------------------
class SelectUI;


//-------------------- �N���X��` --------------------
class Select : public Scene {
public:
	//---�֐�
	Select();
	~Select();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	//---�ϐ�
	std::unique_ptr<SelectUI> pUI;
};



#endif