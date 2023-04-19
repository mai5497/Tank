//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

#define MAX_HP				(3)			// HP最大値

//-------------------- クラス定義 --------------------
class Player : public GameObject {
public:
	Player();
	~Player();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3& GetPlayerPos();
	int GetHP();
private:
	static std::unique_ptr<CAssimpModel> pMyModel;
	XMFLOAT3 rotDestModel;	// 目的の向き
	double m_animTime;

	int hitPoint;
};

#endif