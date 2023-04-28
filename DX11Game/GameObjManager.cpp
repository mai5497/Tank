//************************************************************************************
// 
// ゲームオブジェクト管理[GameObjManager.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
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
	// ゲームシーンを保存
	pGameScene = _pGameScene;

	// ステージ数を取得
	int stageNum = pGameScene->GetStageNum();

	// 当たり判定管理クラスの初期化
	pCollManager = std::make_shared<Collision>();

	// 敵の数の初期化
	enemySum = 0;
	// 何番目のゲームオブジェクトかの変数初期化
	int gameObjNum = 0;

	//----- プレイヤー初期化 -----
	gameObjNum = AddList(std::make_shared<Player>(pGameScene));
	pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
	
	//----- マップ番号で配置が決まるオブジェクトの初期化 -----
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
				gameObjNum = AddList(std::make_shared<Enemy>(i,j,pGameScene));
				pGameObjects[gameObjNum]->gameObjNum = gameObjNum;
			}
		}
	}
}

//====================================================================================
//
//				終了
//
//====================================================================================
void GameObjManager::Uninit() {
	// オブジェクトの開放
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
	pGameObjects.emplace_back(_pGameobj);	// リストに追加
	if (_isInit) {	// 初期化するかどうかデフォルトはする
		pGameObjects[pGameObjects.size() - 1]->Init();
	}
	// 当たり判定のリストへの追加
	Collision::AddList(_pGameobj);

	// 敵だったら敵の総数を加算
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

	// 敵だったら総数を減算
	if (pGameObjects[_index]->myTag == GameObject::ENEMY) {
		enemySum--;
	}
	// 終了処理
	if (_isUninit) {	// 基本は終了処理を行う
		pGameObjects[_index]->Uninit();
	}

	// リストから削除
	pGameObjects.erase(pGameObjects.begin() + _index);

	// 何番目のオブジェクトかをきえたオブジェクトの分合わせる
	for (int i = _index; i < pGameObjects.size(); i++) {
		pGameObjects[i]->gameObjNum--;
	}

	// 当たり判定のリストから削除
	Collision::DelList(_index);
}
