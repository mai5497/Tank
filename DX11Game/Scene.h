//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SCENE_H__
#define __SCENE_H__

#include "main.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void Init();			// ������
	virtual void Uninit();			// �I������
	virtual void Update();			// �X�V
	virtual void Draw();			// �`��
};

#endif
