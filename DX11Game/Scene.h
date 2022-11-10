//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __SCENE_H__
#define __SCENE_H__

#include "main.h"

class Game;
class Title;
class Result;
class Fade;

class Scene {
public:
	enum eSCENE {
		SCENE_NONE = 0,	// 未設定
		SCENE_TITLE,	// タイトル画面
		//SCENE_MODE,		// モード選択画面
		//SCENE_HOWTOPLAY,// 遊び方 
		SCENE_GAME,		// ゲーム画面
		SCENE_RESULT,	// 結果発表
		//SCENE_RANKING,	// ランキング	

		MAX_SCENE		// シーン総数
	};

	Scene();
	~Scene();

	void Init();			// 初期化
	void Uninit();			// 終了処理
	void Update();			// 更新
	void Draw();			// 描画



	static void SetScene(eSCENE eScene);	// 切り替え
	void ChangeScene();
	eSCENE GetScene();					// シーン番号返す

private:

	std::shared_ptr<Fade> pFade;
	static eSCENE nowScene;
	eSCENE oldScene;

protected:

	// とりあえずうごかすためにポインタ作っとく
	//std::shared_ptr <Scene> pGame;
	//std::shared_ptr<Title> pTitle;
	//std::shared_ptr<Result> pResult;

	std::shared_ptr<Scene> pNowScene;

};

#endif
