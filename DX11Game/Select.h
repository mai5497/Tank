//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __SELECT_H__
#define __SELECT_H__

#include "main.h"
#include "Scene.h"

//----- �O����` -----
class BG;

//===========================
//
//	�N���X��`
//
//===========================
class Select : public Scene {
public:
	Select();
	~Select();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	std::unique_ptr<BG> pBG;					// �w�i

};



#endif