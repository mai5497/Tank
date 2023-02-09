//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
// �w�i�`�� [bg.h]
#ifndef __BG_H__
#define __BG_H__
#include "main.h"

#include "SceneManager.h"

class Texture; 

//===========================
//
//	�N���X��`
//
//===========================
class BG {
public:
	BG();
	~BG();

	static HRESULT Init();				// ������
	static void Uninit();				// �I������
	static void Draw(eSCENE _nowScene);	// �`��
private:
	static std::unique_ptr<Texture> pTitleBG;
	static std::unique_ptr<Texture> pSelectBG;
	static std::unique_ptr<Texture> pGameBG;
};
#endif