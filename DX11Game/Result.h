//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

#include "main.h"
#include "Scene.h"

//===========================
//
//	�v���g�^�C�v�錾
//
//===========================
class Result : public Scene {
public:
	Result();
	~Result();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��
};
#endif

