#ifndef __PRESSSPACE_H__
#define __PRESSSPACE_H__

#include "main.h"
#include "polygon.h"
#include "Texture.h"


class PressSpace {
public:
	PressSpace();
	~PressSpace();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static LPCWSTR pszTexFName;

	static std::unique_ptr<Texture> pTexture;
};



#endif
