//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

//-------------------- �C���N���[�h --------------------
#include "GameObject.h"

//-------------------- �O����` --------------------
class BulletLine;

//-------------------- �N���X��` --------------------
class Enemy : public GameObject {
public:
	Enemy(int mapIndex_x, int mapindex_y);
	~Enemy();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Destroy();
	void SetRootIndex(XMINT2 index);
	XMINT2 SetStartIndex();
	bool GetUse();
private:
	static std::unique_ptr<CAssimpModel> pMyModel;

	XMFLOAT3 rotDest;	// �ړI�̌���
	int	bulletTimer;	// �e���˃^�C�}�[
	int	rootTimer;		// ���[�g�����^�C�}�[
	std::vector<XMINT2>::iterator rootIndexNum;	// �����������[�g�̉��Ԗڂ��Ɏg��
	std::vector<XMINT2> rootIndex;	// �����������[�g���i�[�����

	std::unique_ptr<BulletLine> pBulletLine;
};

#endif