//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __PLAYER_H__
#define __PLAYER_H__
//-------------------- インクルード --------------------
#include "GameObject.h"

//-------------------- 定数 --------------------
#define MAX_HP				(3)			// HP最大値

//-------------------- 前方定義 --------------------
class Game;
class BulletLine;

//-------------------- クラス定義 --------------------
class Player : public GameObject {
public:
	//---関数
	Player(Game* _pGameScene);
	~Player();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3& GetPlayerPos();
private:
	//---変数
	static std::unique_ptr<CAssimpModel> pMyModel;
	XMFLOAT3 rotDestModel;	// 目的の向き
	double m_animTime;

	Game* pGameScene;	// ゲームシーン

	int hitPoint;	// HP

	std::unique_ptr<BulletLine> pBulletLine;

};

#endif