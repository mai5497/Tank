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
//				コンストラクタ
//
//====================================================================================
GameObjManager::GameObjManager() {
}


//====================================================================================
//
//				デストラクタ
//
//====================================================================================
GameObjManager::~GameObjManager() {

}

//====================================================================================
//
//				初期化
//
//====================================================================================
void GameObjManager::Init(Game* _pGameScene) {
	pGameScene = _pGameScene;
	int stageNum = pGameScene->GetStageNum();

	// 当たり判定管理クラスの初期化
	pCollManager = std::make_shared<Collision>();

	enemySum = 0;
	int gameObjNum = 0;
	// プレイヤー初期化
	gameObjNum = AddList(std::make_shared<Player>(pGameScene));
	pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
	

	for (int j = 0; j < MAPHEIGHT; j++) {
		for (int i = 0; i < MAPWIDTH; i++) {
			std::shared_ptr<GameObject> work;
			if (WallObj::wallMap[stageNum][j][i] == 1) {
				// オブジェクトの壁初期化
				work = std::make_shared<WallObj>(i, j);
				gameObjNum = AddList(work);
				pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
			} else if (WallObj::wallMap[stageNum][j][i] == 2) {
				// 敵初期化
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
//				終了
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
//				更新
//
//====================================================================================
void GameObjManager::Update() {
	// オブジェクトの更新
	for (updateIndex = 0; updateIndex < pGameObjects.size(); updateIndex++) {
		pGameObjects[updateIndex]->Update();
	}

	// 当たり判定の更新
	pCollManager->Update();

	// ゲームシーンに敵の現在の数を保存する
	pGameScene->StorageEnemySum(enemySum);
}

//====================================================================================
//
//				描画
//
//====================================================================================
void GameObjManager::Draw() {
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	for (int i = 0; i < pGameObjects.size(); i++) {

		pGameObjects[i]->Draw();

	}
}


//====================================================================================
//
//				敵の現在の総数
//
//====================================================================================
int GameObjManager::GetEnemySum() {
	return enemySum;
}

//====================================================================================
//
//				オブジェクトリストの追加
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
//				オブジェクトリストの削除
//
//====================================================================================
void GameObjManager::DelList(int _index, bool _isUninit) {
	updateIndex = _index - 1;	// 今更新しているところがeraseすることにより消えて、消えたところは詰められて更新がうまく回らなくなるため

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
