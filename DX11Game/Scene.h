//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __SCENE_H__
#define __SCENE_H__

#include "main.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void Init();			// 初期化
	virtual void Uninit();			// 終了処理
	virtual void Update();			// 更新
	virtual void Draw();			// 描画
};

#endif
