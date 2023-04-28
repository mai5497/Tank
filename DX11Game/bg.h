//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __BG_H__
#define __BG_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

#include "SceneManager.h"

//-------------------- �O����` --------------------
class Texture; 

//-------------------- �w�i --------------------
class BG {
public:
	//---�֐�
	BG();
	~BG();

	static HRESULT Init();				// ������
	static void Uninit();				// �I������
	static void Draw(eSCENE _nowScene);	// �`��
private:
	//---�ϐ�
	static std::unique_ptr<Texture> pSkyBG;
	static std::unique_ptr<Texture> pSelectBG;
};
#endif