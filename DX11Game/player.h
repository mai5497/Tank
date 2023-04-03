//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

//-------------------- �N���X��` --------------------
class Player : public GameObject {
public:
	Player();
	~Player();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3& GetPlayerPos();
	bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
	bool CollisionPlayer(GameObject collision);
	XMINT2 SetPlayerIndex();

private:
	static std::unique_ptr<CAssimpModel> pMyModel;
	XMFLOAT3 rotDestModel;	// �ړI�̌���
	double m_animTime;

	XMFLOAT3 testPos;
};

#endif