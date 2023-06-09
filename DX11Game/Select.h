//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __SELECT_H__
#define __SELECT_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "Scene.h"

//-------------------- 前方定義 --------------------
class SelectUI;


//-------------------- クラス定義 --------------------
class Select : public Scene {
public:
	//---関数
	Select();
	~Select();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	//---変数
	std::unique_ptr<SelectUI> pUI;
};



#endif