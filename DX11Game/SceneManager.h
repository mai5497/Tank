//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- 前方定義 --------------------
class Scene;
class Fade;

//-------------------- 列挙定数 --------------------
enum eSCENE {
	SCENE_NONE = 0,		// 未設定
	SCENE_TITLE,		// タイトル画面
	SCENE_MODESELECT,	// モード選択画面
	SCENE_TUTORIAL,		// 遊び方 
	SCENE_STAGESUMMARY,	// ステージの説明 
	SCENE_GAME,			// ゲーム画面
	SCENE_RESULT,		// 結果発表

	MAX_SCENE		// シーン総数
};

//-------------------- クラス定義 --------------------
class SceneManager {
public:
	//---関数
	SceneManager();
	~SceneManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetScene(eSCENE eScene);	// 切り替え
	eSCENE GetScene();					// シーン番号返す

private:
	//---関数
	void ChangeScene();

	//---変数
	static eSCENE nextScene;			// 次のシーンのステート
	eSCENE nowScene;					// 今のシーンのステート
	int stageNum;						// ステージの番号

	std::shared_ptr<Fade> pFade;		// フェード

	std::shared_ptr<Scene> pNowScene;	// 今のシーン
};


#endif