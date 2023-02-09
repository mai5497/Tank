//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __SELECT_UI_H__
#define __SELECT_UI_H__
#include "main.h"

class Texture;

//===========================
//
//	クラス定義
//
//===========================
class SelectUI {
public:
	enum SELECT{
		START = 0,
		TUTORIAL,
		SCORE,

		MAX
	};
	SelectUI();
	~SelectUI();

	HRESULT Init();					// 初期化
	void Uninit();					// 終了処理
	int Update();					// 更新
	void Draw();	// 描画
private:
	static std::unique_ptr<Texture> pUI;
	static std::unique_ptr<Texture> pBox;

	int selectNum;
};
#endif