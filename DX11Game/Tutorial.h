//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __TUTORIAL_H__
#define __TUTORIAL_H__

#include "main.h"
#include "Scene.h"

//----- 前方定義 ------
class Texture;

//===========================
//
//	クラス定義
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
	static std::unique_ptr<Texture> pUI;	// 操作説明の画像
};

#endif