//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __HP_UI_H__
#define __HP_UI_H__


//-------------------- �C���N���[�h�� --------------------
#include "main.h"
#include "polygon.h"
#include "Texture.h"

//-------------------- �O����` --------------------
class Game;

//-------------------- �N���X��` --------------------
class HPUI {
public:
	//---�֐�
	HPUI();
	~HPUI();

	void Init(Game* _pGameScene);
	void Uninit();
	void Update();
	void Draw();

private:
	//---�ϐ�
	static std::unique_ptr<Texture> pEmpty;		// ���܂��Ă����UI
	static std::unique_ptr<Texture> pFull;		// ��̕���UI

	Game* pGameScene;							// �Q�[���V�[��
	int playerHP;								// �v���C���[��HP
};

#endif // !__HP_UI_H__