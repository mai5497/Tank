//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __TUTORIAL_H__
#define __TUTORIAL_H__

#include "main.h"
#include "Scene.h"

//----- �O����` ------
class Texture;

//===========================
//
//	�N���X��`
//
//===========================
class Tutorial : public Scene {
public:
	Tutorial();
	~Tutorial();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


private:
	static std::unique_ptr<Texture> pUI;	// ��������̉摜
};

#endif