#ifndef __HP_UI_H__
#define __HP_UI_H__

#include "main.h"
#include "polygon.h"
#include "Texture.h"

//---‘O•û’è‹`
class Game;


class HPUI {
public:
	HPUI();
	~HPUI();

	void Init(Game* _pGameScene);
	void Uninit();
	void Update();
	void Draw();

private:
	static std::unique_ptr<Texture> pEmpty;
	static std::unique_ptr<Texture> pFull;

	Game* pGameScene;
	int playerHP;
};



#endif // !__HP_UI_H__