//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
// �w�i�`�� [bg.h]
#ifndef __BG_H__
#define __BG_H__
#include "main.h"

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

	HRESULT Init();	// ������
	void Uninit();	// �I������
	void Update();	// �X�V
	void Draw();		// �`��
private:
	static std::unique_ptr<Texture> pBG;
};
#endif