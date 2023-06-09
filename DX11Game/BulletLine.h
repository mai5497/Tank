//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __BULLETLINE_H__
#define __BULLETLINE_H__


//-------------------- �C���N���[�h�� --------------------
#include "main.h"

#include "mesh.h"

//-------------------- �O����` --------------------
class GameObject;

//-------------------- �N���X��` --------------------
class BulletLine {
public:
	//---�֐�
	BulletLine();
	~BulletLine();

	void Init(GameObject* _pObj);
	void Uninit();
	void Update();
	void Draw();

	void SetDir(XMFLOAT3 _dir);
private:
	//---�֐�
	void MakeVertexBullet(ID3D11Device* pDevice);
	
	//---�ϐ�
	MESH line;			// �K�C�h�̐��̃��b�V��
	MATERIAL mat;		// �}�e���A��

	GameObject* pObj;	// �o���I�u�W�F�N�g��ۑ�����
		
	XMFLOAT3 pos;		// ���W
	XMFLOAT3 dir;		// �����������
	XMFLOAT3 angle;		// �e�N�X�`�����̂̉�]
};

#endif // !__BULLETLINE_H__