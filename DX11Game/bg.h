//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __BG_H__
#define __BG_H__

//-------------------- インクルード部 --------------------
#include "main.h"

#include "SceneManager.h"

//-------------------- 前方定義 --------------------
class Texture; 

//-------------------- 背景 --------------------
class BG {
public:
	//---関数
	BG();
	~BG();

	static HRESULT Init();				// 初期化
	static void Uninit();				// 終了処理
	static void Draw(eSCENE _nowScene);	// 描画
private:
	//---変数
	static std::unique_ptr<Texture> pSkyBG;
	static std::unique_ptr<Texture> pSelectBG;
};
#endif