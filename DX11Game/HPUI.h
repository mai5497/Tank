//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __HP_UI_H__
#define __HP_UI_H__


//-------------------- インクルード部 --------------------
#include "main.h"
#include "polygon.h"
#include "Texture.h"

//-------------------- 前方定義 --------------------
class Game;

//-------------------- クラス定義 --------------------
class HPUI {
public:
	//---関数
	HPUI();
	~HPUI();

	void Init(Game* _pGameScene);
	void Uninit();
	void Update();
	void Draw();

private:
	//---変数
	static std::unique_ptr<Texture> pEmpty;		// たまってる方のUI
	static std::unique_ptr<Texture> pFull;		// 空の方のUI

	Game* pGameScene;							// ゲームシーン
	int playerHP;								// プレイヤーのHP
};

#endif // !__HP_UI_H__