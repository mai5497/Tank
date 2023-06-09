//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __FADE_H__
#define __FADE_H__


//-------------------- インクルード部 --------------------
#include "SceneManager.h"

//-------------------- 列挙定数 --------------------
enum eFade {
	FADE_NONE = 0,		// 何もしていない
	FADE_IN,			// フェードイン処理中
	FADE_OUT,			// フェードアウト処理中

	MAX_FADE			// フェード最大値
};

//-------------------- クラス定義 --------------------
class Fade {
public:
	//---関数
	Fade();
	~Fade();

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	static void StartFadeOut(eSCENE eScene);			// フェードアウト開始

	eFade GetFade();									// 状態取得
	void SetFadeColor(float fR, float fG, float fB);	// フェードアウト色設定

private:
	//---変数
	float red;		// フェードアウト色
	float green;
	float blue;
	static float alpha;

	static eFade stateFade;
	eFade oldStateFade;
	static eSCENE nextScene;

};
#endif // !__FADE_H__