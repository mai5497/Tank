//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __GAME_SUMMARY_H__
#define __GAME_SUMMARY_H__

#include "main.h"
#include "Scene.h"

//----- �O����` ------
class Texture;

//===========================
//
//	�N���X��`
//
//===========================
class GameSummary : public Scene {
public:
	GameSummary();
	~GameSummary();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


private:
	static std::unique_ptr<Texture> pBG;	// �w�i

	int sceneChangeTimer;
};

#endif