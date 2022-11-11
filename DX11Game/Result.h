//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

#include "main.h"
#include "Scene.h"

//===========================
//
//	プロトタイプ宣言
//
//===========================
class Result : public Scene {
public:
	Result();
	~Result();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画
};
#endif

