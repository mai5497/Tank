//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

#ifndef __PRESSSPACE_H__
#define __PRESSSPACE_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"
#include "polygon.h"
#include "Texture.h"

//-------------------- �N���X��` --------------------
class PressSpace {
public:
	//---�֐�
	PressSpace();
	~PressSpace();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//---�ϐ�
	static LPCWSTR pszTexFName;

	static std::unique_ptr<Texture> pTexture;
};



#endif
