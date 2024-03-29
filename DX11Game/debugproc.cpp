//************************************************************************************
// 
// デバッグログ[debugproc.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************
//-------------------- インクルード部 --------------------
#include <stdarg.h>
#include <string.h>
#include "debugproc.h"
#include "Texture.h"

//-------------------- 定数定義 --------------------
#define	TEXTURE_FILENAME	L"data/texture/PressStart2Ph_e.png"	// 読み込むテクスチャファイル名
#define FONT_WIDTH			16
#define FONT_HEIGHT			16

//-------------------- グローバル変数定義 --------------------
static std::unique_ptr<Texture> g_pTexture;
static char							g_szDebug[8192] = { '\0' };	// デバッグ情報
static bool							g_bHiragana = false;		// 平仮名フラグ

//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT InitDebugProc(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	g_pTexture = std::make_unique<Texture>();
	hr = g_pTexture->SetTexture(pDevice, TEXTURE_FILENAME);

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void UninitDebugProc(void)
{
	// テクスチャ解法
	g_pTexture->ReleaseTexture();
	g_pTexture.reset();
}

//====================================================================================
//
//				更新
//
//====================================================================================
void UpdateDebugProc(void)
{
	// (何もしない)
}

//====================================================================================
//
//				描画
//
//====================================================================================
void DrawDebugProc(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMFLOAT2 vPos(SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f - FONT_HEIGHT * 0.5f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetPolygonTexture(g_pTexture->GetTexture());
	SetPolygonFrameSize(8.0f / 128.0f, 8.0f / 128.0f);
	SetPolygonSize(FONT_WIDTH, FONT_HEIGHT);
	for (char* pChr = &g_szDebug[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f;
			vPos.y -= FONT_HEIGHT;
			continue;
		}
		SetPolygonPos(vPos.x, vPos.y);
		int nChr = (BYTE)*pChr;
		if (g_bHiragana &&
			(nChr >= 0xa6 && nChr <= 0xaf || nChr >= 0xb1 && nChr <= 0xdd))
			nChr ^= 0x20;
		SetPolygonUV((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f);
		UpdatePolygon();
		DrawPolygon(pDeviceContext);
		vPos.x += FONT_WIDTH;
	}
	// テクスチャ設定を元に戻す
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}

//====================================================================================
//
//				登録開始
//
//====================================================================================
void StartDebugProc(bool hiragana)
{
	SetHiragana(hiragana);
	g_szDebug[0] = '\0';
}

//====================================================================================
//
//				登録
//
//====================================================================================
void PrintDebugProc(const char *fmt, ...)
{
	va_list list;
	char aBuf[256];

	va_start(list, fmt);
	_vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// 連結
	if ((strlen(g_szDebug) + strlen(aBuf)) < sizeof(g_szDebug) - 1) {
		strcat_s(g_szDebug, sizeof(g_szDebug), aBuf);
	}
}

bool SetHiragana(bool hiragana)
{
	bool prev = g_bHiragana;
	g_bHiragana = hiragana;
	return prev;
}
