//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __SELECT_UI_H__
#define __SELECT_UI_H__

//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- 前方定義 --------------------
class Texture;

//-------------------- クラス定義 --------------------
class SelectUI {
public:
	//---列挙定数
	enum SELECT{
		START = 0,
		TUTORIAL,

		MAX
	};

	//---関数
	SelectUI();
	~SelectUI();

	HRESULT Init();					// 初期化
	void Uninit();					// 終了処理
	int Update();					// 更新
	void Draw();	// 描画
private:
	//---変数
	static std::unique_ptr<Texture> pUI;
	static std::unique_ptr<Texture> pBox;

	int selectNum;		// 選択番号
};
#endif