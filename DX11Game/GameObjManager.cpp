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

	// �����蔻��Ǘ��N���X�̏�����
	pCollManager = std::make_shared<Collision>();

	int gameObjNum;
	// �v���C���[������
	gameObjNum = AddList(std::make_shared<Player>());
	pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
	// �I�u�W�F�N�g�̕Ǐ�����
	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			std::shared_ptr<GameObject> work;
			work = std::make_shared<WallObj>(i, j);
			if (WallObj::wallMap[j][i] != 0) {
				gameObjNum = AddList(work);
				pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
			}
		}
	}
	// �G������
	enemySum = 0;
	for (int i = 0; i < MAX_ENEMY; i++) {
		gameObjNum = AddList(std::make_shared<Enemy>());
		pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
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

	if (enemySum < 1) {
		Fade::StartFadeOut(SCENE_RESULT);
	}
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
//				���j���擾
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
