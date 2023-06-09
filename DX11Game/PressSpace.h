//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __PRESSSPACE_H__
#define __PRESSSPACE_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "polygon.h"
#include "Texture.h"

//-------------------- クラス定義 --------------------
class PressSpace {
public:
	//---関数
	PressSpace();
	~PressSpace();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//---変数
	static LPCWSTR pszTexFName;

	static std::unique_ptr<Texture> pTexture;
};



#endif
