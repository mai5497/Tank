//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

#include "main.h"
#include "Scene.h"

class Texture;

//===========================
//
//	�v���g�^�C�v�錾
//
//===========================
class Result : public Scene {
public:
	enum TEXNUM {
		TEX_BG = 0,
		TEX_TIMER,

		MAX_TEXTURE,
	};

	Result(int _stageNum);
	~Result();

	virtual void Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��

private:
	static std::unique_ptr<Texture> pFrame;
	static std::unique_ptr<Texture> pAllClear;
	static std::unique_ptr<Texture> pStageChar;


	int stageNum;

};
#endif

