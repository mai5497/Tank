//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "main.h"
#include "GameObject.h"


#ifdef _DEBUG
#define MAX_ENEMY			(1)		// �G�ő�(�f�o�b�O�p)
#else
#define MAX_ENEMY			(1)	// �G�ő�(�ق񂿂��)
#endif

//-------------------- �N���X��` --------------------
class Enemy : public GameObject {
public:
	Enemy();
	~Enemy();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	int CollisionEnemy(XMFLOAT3 pos, float radius, float damage);
	void CollisionEnemy(GameObject collision);
	int GetEnemyKillSum();
	void SetRootIndex(XMINT2* index);
	XMINT2 SetStartIndex(int enemyNo);
private:
	//XMFLOAT3	pos;	// ���݂̈ʒu
	XMFLOAT3 rot;		// ���݂̌���
	XMFLOAT3 rotDest;	// �ړI�̌���
	//XMFLOAT3	move;	// �ړ���
	//XMFLOAT3	size;	// �傫��
	bool use;			// �g�p�����ǂ���
	int	bulletTimer;	// �e���˃^�C�}�[
	int	rootTimer;		// ���[�g�����^�C�}�[
	XMINT2*	mapIndex;	// ���W���}�b�v�ԍ��ŕ\��������yx�̏��Ɋi�[

	//XMFLOAT4X4	m_mtxWorld;		// ���[���h�}�g���b�N�X

	int	shadowNum;		// �ۉe�ԍ�

};

#endif