//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
// 背景描画 [bg.h]
#ifndef __BG_H__
#define __BG_H__
#include "main.h"

#include "SceneManager.h"

class Texture; 

//===========================
//
//	クラス定義
//
//===========================
class BG {
public:
	BG();
	~BG();

	static HRESULT Init();				// 初期化
	static void Uninit();				// 終了処理
	static void Draw(eSCENE _nowScene);	// 描画
private:
	static std::unique_ptr<Texture> pTitleBG;
	static std::unique_ptr<Texture> pSelectBG;
	static std::unique_ptr<Texture> pGameBG;
};
#endif