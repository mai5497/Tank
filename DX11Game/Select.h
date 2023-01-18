//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __SELECT_H__
#define __SELECT_H__

#include "main.h"
#include "Scene.h"

//----- 前方定義 -----
class BG;

//===========================
//
//	クラス定義
//
//===========================
class Select : public Scene {
public:
	Select();
	~Select();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	std::unique_ptr<BG> pBG;					// 背景

};



#endif