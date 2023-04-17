//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __GAME_SUMMARY_H__
#define __GAME_SUMMARY_H__

#include "main.h"
#include "Scene.h"

//----- 前方定義 ------
class Texture;

//===========================
//
//	クラス定義
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
	static std::unique_ptr<Texture> pBG;	// 背景

	int sceneChangeTimer;
};

#endif