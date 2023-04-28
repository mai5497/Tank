//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __GAME_H__
#define __GAME_H__

//-------------------- �C���N���[�h --------------------
#include "main.h"
#include "Scene.h"

//-------------------- �O����` --------------------
class GameObjManager;
class MeshField;
class HPUI;

//-------------------- �N���X��` --------------------
class Game : public Scene {
public:
	//---�֐�
	Game(int _stageNum);
	~Game();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	int GetStageNum();

	void StoragePlayerHP(int _hp);
	int GetPlayerHP();

	void StorageEnemySum(int _enemySum);

	void StoragePlayerPos(XMFLOAT3 _pos);
	XMFLOAT3 GetPlayerPos();
private:
	//---�ϐ�
	std::unique_ptr<GameObjManager> pGameObjects;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X
	std::unique_ptr<MeshField> pMeshField;			// �t�B�[���h

	std::unique_ptr<HPUI> pHPUI;					// HP��UI

	int stageNum;									// ���݂̃X�e�[�W�ԍ�

	int playerHP;									// �v���C���[��HP��ۑ����Ă���
	int enemySum;									// �G�̎c����ۑ�����
	XMFLOAT3 playerPos;								// �v���C���[�̍��W��ۑ����Ă���
};

#endif