//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SELECT_UI_H__
#define __SELECT_UI_H__
#include "main.h"

class Texture;

//===========================
//
//	�N���X��`
//
//===========================
class SelectUI {
public:
	enum SELECT{
		START = 0,
		TUTORIAL,
		SCORE,

		MAX
	};
	SelectUI();
	~SelectUI();

	HRESULT Init();					// ������
	void Uninit();					// �I������
	int Update();					// �X�V
	void Draw();	// �`��
private:
	static std::unique_ptr<Texture> pUI;
	static std::unique_ptr<Texture> pBox;

	int selectNum;
};
#endif