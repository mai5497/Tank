//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

#include "main.h"
#include "Scene.h"

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
};

#endif // !__TITLE_H__

