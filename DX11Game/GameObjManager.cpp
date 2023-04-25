#include "GameObjManager.h"

#include "fade.h"

#include "Collision.h"
#include "Player.h"
#include "WallObject.h"
#include "Enemy.h"

#include "Game.h"


std::vector<std::shared_ptr<GameObject>> GameObjManager::pGameObjects;
int GameObjManager::enemySum;
int GameObjManager::updateIndex;

//====================================================================================
//
//				�R���X�g���N�^
//
//====================================================================================
GameObjManager::GameObjManager() {
}


//====================================================================================
//
//				�f�X�g���N�^
//
//====================================================================================
GameObjManager::~GameObjManager() {

}

//====================================================================================
//
//				������
//
//====================================================================================
void GameObjManager::Init(Game* _pGameScene) {
	pGameScene = _pGameScene;
	int stageNum = pGameScene->GetStageNum();

	// �����蔻��Ǘ��N���X�̏�����
	pCollManager = std::make_shared<Collision>();

	enemySum = 0;
	int gameObjNum = 0;
	// �v���C���[������
	gameObjNum = AddList(std::make_shared<Player>(pGameScene));
	pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
	

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			std::shared_ptr<GameObject> work;
			if (WallObj::wallMap[stageNum][j][i] == 1) {
				// �I�u�W�F�N�g�̕Ǐ�����
				work = std::make_shared<WallObj>(i, j);
				gameObjNum = AddList(work);
				pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
			} else if (WallObj::wallMap[stageNum][j][i] == 2) {
				// �G������
				gameObjNum = AddList(std::make_shared<Enemy>(i,j));
				pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
			}
		}
	}


	//for (int i = 0; i < pGameObjects.size(); i++) {
	//	pGameObjects[i]->Init();
	//	pCollManager->AddList(pGameObjects[i]);
	//}

}

//====================================================================================
//
//				�I��
//
//====================================================================================
void GameObjManager::Uninit() {
	int delCnt = pGameObjects.size();
	for (int i = 0; i < delCnt; i++) {
		pGameObjects[i]->Uninit();
		pGameObjects[i].reset();
	}

	pGameObjects.clear();
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void GameObjManager::Update() {
	// �I�u�W�F�N�g�̍X�V
	for (updateIndex = 0; updateIndex < pGameObjects.size(); updateIndex++) {
		pGameObjects[updateIndex]->Update();
	}

	// �����蔻��̍X�V
	pCollManager->Update();

	// �Q�[���V�[���ɓG�̌��݂̐���ۑ�����
	pGameScene->StorageEnemySum(enemySum);
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void GameObjManager::Draw() {
	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	for (int i = 0; i < pGameObjects.size(); i++) {

		pGameObjects[i]->Draw();

	}
}


//====================================================================================
//
//				�G�̌��݂̑���
//
//====================================================================================
int GameObjManager::GetEnemySum() {
	return enemySum;
}

//====================================================================================
//
//				�I�u�W�F�N�g���X�g�̒ǉ�
//
//====================================================================================
int GameObjManager::AddList(const std::shared_ptr<GameObject>& _pGameobj, bool _isInit) {
	pGameObjects.emplace_back(_pGameobj);
	if (_isInit) {
		pGameObjects[pGameObjects.size() - 1]->Init();
	}
	Collision::AddList(_pGameobj);

	if (_pGameobj->myTag == GameObject::ENEMY) {
		enemySum++;
	}

	return pGameObjects.size() - 1;
}

//====================================================================================
//
//				�I�u�W�F�N�g���X�g�̍폜
//
//====================================================================================
void GameObjManager::DelList(int _index, bool _isUninit) {
	updateIndex = _index - 1;	// ���X�V���Ă���Ƃ��낪erase���邱�Ƃɂ������āA�������Ƃ���͋l�߂��čX�V�����܂����Ȃ��Ȃ邽��

	if (pGameObjects[_index]->myTag == GameObject::ENEMY) {
		enemySum--;
	}
	if (_isUninit) {
		pGameObjects[_index]->Uninit();
	}

	pGameObjects.erase(pGameObjects.begin() + _index);

	for (int i = _index; i < pGameObjects.size(); i++) {
		pGameObjects[i]->gameObjNum--;
	}
	Collision::DelList(_index);
}
