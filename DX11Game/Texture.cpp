//************************************************************************************
// 
// テクスチャ読込[Texture.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdlib.h>
#include "Texture.h"

#pragma comment(lib, "DirectXTex")

//-------------------- 名前空間定義 --------------------
using namespace DirectX;

//====================================================================================
//
//				コンストラクタ
//
//====================================================================================
Texture::Texture() {
	pTexture = nullptr;
}

//====================================================================================
//
//				
//
//====================================================================================
Texture::~Texture() {

}


//====================================================================================
//
//				テクスチャ開放
//
//====================================================================================
void Texture::ReleaseTexture() {
	SAFE_RELEASE(pTexture);
}

//====================================================================================
//
//				テクスチャ設定(ファイル名から取得して設定)
//
//====================================================================================
HRESULT Texture::SetTexture(_In_ ID3D11Device* _d3dDevice, _In_z_ const wchar_t* _szFileName) {
	return CreateTextureFromFile(_d3dDevice, _szFileName);
}
HRESULT Texture::SetTexture(_In_ ID3D11Device* _d3dDevice, _In_z_ const char* _szFileName) {
	return CreateTextureFromFile(_d3dDevice, _szFileName);
}
HRESULT Texture::SetTexture(_In_ ID3D11Device* _d3dDevice, _In_bytecount_(wicDataSize) const uint8_t* _wicData, size_t _wicDataSize) {
	return CreateTextureFromMemory(_d3dDevice, _wicData, _wicDataSize);
}


//====================================================================================
//
//				テクスチャ設定(直接ファイルを受け取る)
//
//====================================================================================
HRESULT Texture::SetTexture(ID3D11ShaderResourceView* _pTexture) {
	if (_pTexture) {
		pTexture = _pTexture;
		return S_OK;
	} else {
		MessageBox(NULL, _T("渡されたテクスチャが空です。\nTexture.cpp(55)"), _T("error"), MB_OK);
		return S_FALSE;
	}
}


//====================================================================================
//
//				テクスチャゲッター
//
//====================================================================================
ID3D11ShaderResourceView* Texture::GetTexture() const{
	return pTexture;
}


//====================================================================================
//
//				テクスチャ取得(メモリから)
//
//====================================================================================
HRESULT Texture::CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
	_In_bytecount_(wicDataSize) const uint8_t* wicData,
	_In_ size_t wicDataSize,
	_Out_opt_ TexMetadata* pTexInfo)
{
	//*textureView = nullptr;
	pTexture = nullptr;
	TexMetadata meta;
	ScratchImage image;
	HRESULT hr;
	if (wicDataSize >= 18 && memcmp(&wicData[wicDataSize - 18], "TRUEVISION-XFILE.", 18) == 0) {
		hr = LoadFromTGAMemory(wicData, wicDataSize, &meta, image);
	} else if (wicDataSize >= 4 && memcmp(wicData, "DDS ", 4) == 0) {
		hr = LoadFromDDSMemory(wicData, wicDataSize, DDS_FLAGS_FORCE_RGB, &meta, image);
	} else {
		hr = LoadFromWICMemory(wicData, wicDataSize, WIC_FLAGS_FORCE_RGB, &meta, image);
	}
	if (FAILED(hr)) return hr;
	if (pTexInfo) *pTexInfo = meta;
	return CreateShaderResourceView(d3dDevice, image.GetImages(), image.GetImageCount(), meta, &pTexture);
}

//====================================================================================
//
//				テクスチャ設定(ファイルから)
//
//====================================================================================
HRESULT Texture::CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
	_In_z_ const wchar_t* szFileName,
	_Out_opt_ TexMetadata* pTexInfo)
{
	//*textureView = nullptr;
	pTexture = nullptr;
	TexMetadata meta;
	ScratchImage image;
	WCHAR wszExt[_MAX_EXT];
	_wsplitpath(szFileName, nullptr, nullptr, nullptr, wszExt);
	HRESULT hr;
	if (_wcsicmp(wszExt, L".tga") == 0)
		hr = LoadFromTGAFile(szFileName, &meta, image);
	else if (_wcsicmp(wszExt, L".dds") == 0)
		hr = LoadFromDDSFile(szFileName, DDS_FLAGS_FORCE_RGB, &meta, image);
	else
		hr = LoadFromWICFile(szFileName, WIC_FLAGS_FORCE_RGB, &meta, image);
	if (FAILED(hr)) return hr;
	if (pTexInfo) *pTexInfo = meta;
	return CreateShaderResourceView(d3dDevice, image.GetImages(), image.GetImageCount(), meta, &pTexture);
}
HRESULT Texture::CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
	_In_z_ const char* szFileName,
	_Out_opt_ TexMetadata* pTexInfo)
{
	WCHAR wszTexFName[_MAX_PATH];
	int nLen = MultiByteToWideChar(CP_ACP, 0, szFileName, lstrlenA(szFileName), wszTexFName, _countof(wszTexFName));
	if (nLen <= 0) return E_FAIL;
	wszTexFName[nLen] = L'\0';
	return CreateTextureFromFile(d3dDevice, wszTexFName, pTexInfo);
}
