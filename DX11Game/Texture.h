//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

#ifdef _MSC_VER
#pragma once
#endif

#include <d3d11.h>
#include <DirectXTex.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#include "polygon.h"

//===========================
//
//	クラス定義
//
//===========================
class Texture {
public:
	//---関数
	Texture();
	~Texture();

	void ReleaseTexture();
	/*
	* _In_    : 入力専用
	* _Out_   : 出力専用(値を返さないオブジェクトには使えない) 
	* _Inout_ : 関数によって変更される
	* 
	*/
	HRESULT SetTexture(_In_ ID3D11Device* _d3dDevice,_In_z_ const wchar_t* _szFileName);	// ファイル名を受け取り読み取りをするセッター
	HRESULT SetTexture(_In_ ID3D11Device* _d3dDevice,_In_z_ const char* _szFileName);		// ファイル名を受け取り読み取りをするセッター
	HRESULT SetTexture(_In_ ID3D11Device* d3dDevice,_In_bytecount_(wicDataSize) const uint8_t* wicData,_In_ size_t _wicDataSize);	// ファイル名を受け取り読み取りをするセッター
	HRESULT SetTexture(ID3D11ShaderResourceView* _pTexture);								// 直接シェーダーリソースファイルをもらうセッター
	ID3D11ShaderResourceView* GetTexture() const;

	//---変数

private:
	//---関数
	//----- テクスチャをメモリやファイルから生成 -----
	HRESULT CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
		_In_bytecount_(wicDataSize) const uint8_t* wicData,
		_In_ size_t wicDataSize,
		_Out_opt_ DirectX::TexMetadata* pTexInfo = nullptr
	);

	HRESULT CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
		_In_z_ const wchar_t* szFileName,
		_Out_opt_ DirectX::TexMetadata* pTexInfo = nullptr
	);

	HRESULT CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
		_In_z_ const char* szFileName,
		_Out_opt_ DirectX::TexMetadata* pTexInfo = nullptr
	);


	//---変数
	ID3D11ShaderResourceView* pTexture;	// 描画に使うテクスチャ

};