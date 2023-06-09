//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __FADE_H__
#define __FADE_H__


//-------------------- �C���N���[�h�� --------------------
#include "SceneManager.h"

//-------------------- �񋓒萔 --------------------
enum eFade {
	FADE_NONE = 0,		// �������Ă��Ȃ�
	FADE_IN,			// �t�F�[�h�C��������
	FADE_OUT,			// �t�F�[�h�A�E�g������

	MAX_FADE			// �t�F�[�h�ő�l
};

//-------------------- �N���X��` --------------------
class Fade {
public:
	//---�֐�
	Fade();
	~Fade();

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��

	static void StartFadeOut(eSCENE eScene);			// �t�F�[�h�A�E�g�J�n

	eFade GetFade();									// ��Ԏ擾
	void SetFadeColor(float fR, float fG, float fB);	// �t�F�[�h�A�E�g�F�ݒ�

private:
	//---�ϐ�
	float red;		// �t�F�[�h�A�E�g�F
	float green;
	float blue;
	static float alpha;

	static eFade stateFade;
	eFade oldStateFade;
	static eSCENE nextScene;

};
#endif // !__FADE_H__