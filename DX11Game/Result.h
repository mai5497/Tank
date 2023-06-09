//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __RESULT__H__
#define __RESULT__H__

//-------------------- インクルード --------------------
#include "main.h"
#include "Scene.h"

//-------------------- 前方定義 --------------------
class Texture;

//-------------------- クラス定義 --------------------
class Result : public Scene {
public:
	//---関数
	Result(int _stageNum);
	~Result();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	//---変数
	static std::unique_ptr<Texture> pFrame;		// UI枠
	static std::unique_ptr<Texture> pAllClear;	// UIオールクリア
	static std::unique_ptr<Texture> pStageChar;	// UI「ステージ」の文字

	int stageNum;								// ステージの番号

};
#endif

