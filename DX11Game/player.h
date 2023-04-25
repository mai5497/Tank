//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __PLAYER_H__
#define __PLAYER_H__
//-------------------- �C���N���[�h --------------------
#include "GameObject.h"

//-------------------- �萔 --------------------
#define MAX_HP				(3)			// HP�ő�l

//-------------------- �O����` --------------------
class Game;
class BulletLine;

//-------------------- �N���X��` --------------------
class Player : public GameObject {
public:
	//---�֐�
	Player(Game* _pGameScene);
	~Player();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMFLOAT3& GetPlayerPos();
private:
	//---�ϐ�
	static std::unique_ptr<CAssimpModel> pMyModel;
	XMFLOAT3 rotDestModel;	// �ړI�̌���
	double m_animTime;

	Game* pGameScene;	// �Q�[���V�[��

	int hitPoint;	// HP

	std::unique_ptr<BulletLine> pBulletLine;

};

#endif