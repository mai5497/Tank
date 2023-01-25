//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __TITLE_H__
#define __TITLE_H__

#include "main.h"
#include "Scene.h"

class TitleLogo;
class PressSpace;
class BG;
class MeshField;
class Particle;
class Texture;

//===========================
//
//	クラス定義
//
//===========================
class Title : public Scene {
public:
	Title();
	~Title();

	virtual void Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画

private:
	std::unique_ptr<TitleLogo> pLogo;			// タイトルロゴ
	std::unique_ptr<PressSpace> pStart;			// プレススペース
	std::unique_ptr<BG> pBG;					// 背景
	std::unique_ptr<MeshField> pMeshField;		// フィールド
	std::unique_ptr<Particle> pParticle;		// パーティクル

	int timer;
	int fadeTimer;
	bool isFade;
};

#endif // !__TITLE_H__

