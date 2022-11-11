//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

#include "main.h"
#include "Scene.h"

//===========================
//
//	クラス定義
//
//===========================
class Title : public Scene {
public:
	Title();
	~Title();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画
};

#endif // !__TITLE_H__

