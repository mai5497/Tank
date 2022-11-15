//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "main.h"
#include "GameObject.h"

//-------------------- �N���X��` --------------------
class Enemy : public GameObject {
public:
	Enemy();
	~Enemy();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	//int CollisionEnemy(XMFLOAT3 pos, float radius, float damage);
	//void CollisionEnemy(GameObject collision);
	void SetRootIndex(XMINT2 index);
	XMINT2 SetStartIndex();
	bool GetUse();
private:
	//XMFLOAT3	pos;	// ���݂̈ʒu
	XMFLOAT3 rotDest;	// �ړI�̌���
	//XMFLOAT3	move;	// �ړ���
	//XMFLOAT3	size;	// �傫��
	bool use;			// �g�p�����ǂ���
	int	bulletTimer;	// �e���˃^�C�}�[
	int	rootTimer;		// ���[�g�����^�C�}�[
	//XMINT2	mapIndex;	// ���W���}�b�v�ԍ��ŕ\��������yx�̏��Ɋi�[

	//XMFLOAT4X4	m_mtxWorld;		// ���[���h�}�g���b�N�X

	int	shadowNum;		// �ۉe�ԍ�

};

#endif