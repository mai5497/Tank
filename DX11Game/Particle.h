//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

//-------------------- インクルード --------------------
#include "main.h"
#include "Texture.h"
#include "mesh.h"

//-------------------- 定数 --------------------
#define PARTICLE_MAX	(20)	// パーティクルの数

//-------------------- クラス定義 --------------------
class Particle {
public:
	Particle();
	~Particle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//---パーティクル関連
	struct PARTICLE {
		int status;					// 状態
		int timer;					// タイマー	
		XMFLOAT2 pos;				// 位置
		float angle;				// 表示角度
		float size;					// 表示サイズ
		XMFLOAT2	velocity;		// 速度
		XMFLOAT2	acceleration;	// 加速度
		XMFLOAT4 color;
	};
	static	MESH		mesh;
	static 	MATERIAL	material;

	PARTICLE particles[PARTICLE_MAX];

private:
	static std::unique_ptr<Texture> pTexture;

	bool isButtonOnce;	// 一度だけ入力をうけつける
};


#endif