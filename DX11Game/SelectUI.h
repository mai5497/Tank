//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SELECT_UI_H__
#define __SELECT_UI_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �O����` --------------------
class Texture;

//-------------------- �N���X��` --------------------
class SelectUI {
public:
	//---�񋓒萔
	enum SELECT{
		START = 0,
		TUTORIAL,

		MAX
	};

	//---�֐�
	SelectUI();
	~SelectUI();

	HRESULT Init();					// ������
	void Uninit();					// �I������
	int Update();					// �X�V
	void Draw();	// �`��
private:
	//---�ϐ�
	static std::unique_ptr<Texture> pUI;
	static std::unique_ptr<Texture> pBox;

	int selectNum;		// �I��ԍ�
};
#endif