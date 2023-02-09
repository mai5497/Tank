#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "main.h"

class Scene;
class Fade;

enum eSCENE {
	SCENE_NONE = 0,		// 未設定
	SCENE_TITLE,		// タイトル画面
	SCENE_MODESELECT,	// モード選択画面
	SCENE_TUTORIAL,	// 遊び方 
	SCENE_GAME,			// ゲーム画面
	SCENE_RESULT,		// 結果発表
	//SCENE_RANKING,	// ランキング	

	MAX_SCENE		// シーン総数
};

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetScene(eSCENE eScene);	// 切り替え
	eSCENE GetScene();					// シーン番号返す

private:
	void ChangeScene();

	static eSCENE nextScene;
	eSCENE nowScene;

	std::shared_ptr<Fade> pFade;

	std::shared_ptr<Scene> pNowScene;
};


#endif