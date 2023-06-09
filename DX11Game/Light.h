//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __LIGHT_H__
#define __LIGHT_H__

//-------------------- インクルード部 --------------------
#include "main.h"


//-------------------- クラス定義 --------------------
class CLight
{
private:
	//---変数
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_specular;
	XMFLOAT3 m_direction;
	bool m_bEnable;			// ライティング有効/無効

public:
	//---関数
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