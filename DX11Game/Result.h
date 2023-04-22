//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

#include "main.h"
#include "Scene.h"

class Texture;

//===========================
//
//	プロトタイプ宣言
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

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	static std::unique_ptr<Texture> pFrame;
	static std::unique_ptr<Texture> pAllClear;
	static std::unique_ptr<Texture> pStageChar;


	int stageNum;

};
#endif

