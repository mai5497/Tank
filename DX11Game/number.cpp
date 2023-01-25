//************************************************************************************
// 
// 数字[number.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "number.h"
#include "Texture.h"

//-------------------- 定数定義 --------------------
#define PATH_NUMBERTEXTURE	L"data/texture/number001.png"


//-------------------- グローバル変数定義 --------------------
static std::unique_ptr<Texture> pTexture;


//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT InitNumber() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();

	pTexture = std::make_unique<Texture>();

	hr = pTexture->SetTexture(pDevice, PATH_NUMBERTEXTURE);
	return hr;
}


//====================================================================================
//
//				終了
//
//====================================================================================
void UninitNumber() {
	// テクスチャ開放
	pTexture->ReleaseTexture();
	pTexture.reset();
}


//====================================================================================
//
//				数字描画
//
//====================================================================================
void DrawNumber(XMFLOAT2 vPos,unsigned uNumber,int nWidth,float fSizeX,float fSizeY) {
	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonColor(1.0f,1.0f, 1.0f);
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonTexture(pTexture->GetTexture());
	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);

	vPos.x += (nWidth - 0.5f) * fSizeX;
	vPos.y -= fSizeY * 0.5f;

	for (; nWidth > 0 ;--nWidth) {
		unsigned n = uNumber % 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV((n % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
					 (n / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
		DrawPolygon(pDC);

		uNumber /= 10;

		vPos.x -= fSizeX;
	}
	//元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}

