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
	~Scene();

	virtual void Init();			// ������
	virtual void Uninit();			// �I������
	virtual void Update();			// �X�V
	virtual void Draw();			// �`��

protected:

	// �Ƃ肠���������������߂Ƀ|�C���^����Ƃ�
	//std::shared_ptr <Scene> pGame;
	//std::shared_ptr<Title> pTitle;
	//std::shared_ptr<Result> pResult;


};

#endif
