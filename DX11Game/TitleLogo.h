//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifndef __TITLELOGO_H__
#define __TITLELOGO_H__

//-------------------- インクルード部 --------------------
#include "main.h"
#include "polygon.h"
#include "Texture.h"

//-------------------- 定数 --------------------
#define SPRING_TOTAL	(4)	// ゴムひも総数
#define STRING_TOTAL	(6)	// 文字数

//-------------------- クラス定義 --------------------
class TitleLogo {
public:
	//---関数
	TitleLogo();
	~TitleLogo();

	void Init();
	void Reset();	// タイトルリピート用関数
	void Uninit();
	void Update();
	void Draw();

private:
	//---変数
	static LPCWSTR pszTexFName; 

	static std::unique_ptr<Texture> pTexture;

	struct SPRING {
		int		status;				// ステータス
		bool	onLock;				// 固定フラグ
		XMFLOAT2 position;			// 位置座標 (Position)
		XMFLOAT2 velocity;			// 速度 (Velocity)
		XMFLOAT2 acceleration;		// 加速度 (Acceleration)
		XMFLOAT2 resultant;			// 合力 (Resultant)
		XMFLOAT2 gravity;			// 重力 (Gravity)
		float	mass;				// ばねの重さ
		XMFLOAT2 tension_up;		// 張力 (Tension) 上方向
		XMFLOAT2 tension_down;		// 張力 (Tension) 下方向
	};

	SPRING springs[STRING_TOTAL][SPRING_TOTAL];
};



#endif