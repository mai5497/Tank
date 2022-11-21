#ifndef __GAMEOBJMANAGER_H__
#define __GAMEOBJMANAGER_H__

#include "main.h"

class GameObject;

class GameObjManager {
public:
	GameObjManager();
	~GameObjManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	std::shared_ptr<GameObject[]> pGameObjects;
};



#endif