#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "main.h"
#include "Texture.h"
#include "mesh.h"


#define PARTICLE_MAX	(20)	// パーティクルの数

class Particle {
public:
	Particle();
	~Particle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

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
	HRESULT MakeVertex(ID3D11Device* pDevice);
};


#endif