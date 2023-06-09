//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __LIGHT_H__
#define __LIGHT_H__

//-------------------- �C���N���[�h�� --------------------
#include "main.h"


//-------------------- �N���X��` --------------------
class CLight
{
private:
	//---�ϐ�
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_specular;
	XMFLOAT3 m_direction;
	bool m_bEnable;			// ���C�e�B���O�L��/����

public:
	//---�֐�
	CLight();

	void Init();
	XMFLOAT4& GetDiffuse() { return m_diffuse; }
	XMFLOAT4& GetAmbient() { return m_ambient; }
	XMFLOAT4& GetSpecular() { return m_specular; }
	XMFLOAT3& GetDir();
	void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }
	void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }

	static CLight* Get();
};

#endif // !__LIGHT_H__