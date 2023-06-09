//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

//-------------------- インクルード --------------------
#include "main.h"
#include "Scene.h"

//-------------------- 前方定義 --------------------
class TitleLogo;
class PressSpace;
class MeshField;
class Particle;
class Texture;

//-------------------- クラス定義 --------------------
class Title : public Scene {
public:
	//---関数
	Title();
	~Title();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	//---変数
	std::unique_ptr<TitleLogo> pLogo;			// タイトルロゴ
	std::unique_ptr<PressSpace> pStart;			// プレススペース
	std::unique_ptr<Particle> pParticle;		// パーティクル

	int resetTimer;	// シーンをリスタートする時間
	int fadeTimer;	// フェードに入るまでの待機時間
	bool isFade;	// フェードに入る許可フラグ
};

#endif // !__TITLE_H__

