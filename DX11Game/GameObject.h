//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"
#include "AssimpModel.h"
#include "collision.h"

//-------------------- �N���X��` --------------------
class GameObject {
public:
	//---�񋓒萔
	enum ObjTag {	// �I�u�W�F�N�g�̎��
		NONE = 0,
		PLAYER,
		ENEMY,
		WALL,
		BULLET_PLAYER,
		BULLET_ENEMY

	};

	//---�֐�
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//---�ϐ�
	XMFLOAT3 pos;						// ���W
	XMFLOAT3 size;						// �`��傫��
	XMFLOAT3 moveVal;					// �ړ���

	XMINT2 mapIndex;					// 0,1�̃}�b�v��������v�f�ԍ������ɂȂ�̂����i�[

	Collision::eCollisionType collType;	// �I�u�W�F�N�g���ÓI�����I��
	float collRadius;					// �X�t�B�A�R���C�_�[�̔��a
	XMFLOAT3 collSize;					// �{�b�N�X�R���C�_�[��xyz�̑傫��(���a)

	XMFLOAT4X4 mtxWorld;				// ���[���h�}�g���b�N�X
	XMFLOAT3   rotModel;				// ���݂̌���
	
	bool isCollision;					// �����蔻��̃I���I�t������
	bool isHit;							// �����ɓ������Ă���

	ObjTag myTag;						// ���̃I�u�W�F�N�g�̃^�O
	std::vector<GameObject> hitList;	// ���̃I�u�W�F�N�g�ɓ������Ă���I�u�W�F�N�g

	bool use;							// �g�p�t���O

	int gameObjNum;						// GameObjManager�ɂ���gameobj�̂肷�Ƃ̉��ԖڂɊi�[����Ă��邩
	
protected:
	int	shadowNum;						// �ۉe�ԍ�

};


#endif