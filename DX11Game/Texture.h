//************************************************************************************
// 
// �ҏW�ҁF�ɒn�c�^��
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
//	�N���X��`
//
//===========================
class Texture {
public:
	//---�֐�
	Texture();
	~Texture();

	void ReleaseTexture();
	/*
	* _In_    : ���͐�p
	* _Out_   : �o�͐�p(�l��Ԃ��Ȃ��I�u�W�F�N�g�ɂ͎g���Ȃ�) 
	* _Inout_ : �֐��ɂ���ĕύX�����
	* 
	*/
	HRESULT SetTexture(_In_ ID3D11Device* _d3dDevice,_In_z_ const wchar_t* _szFileName);	// �t�@�C�������󂯎��ǂݎ�������Z�b�^�[
	HRESULT SetTexture(_In_ ID3D11Device* _d3dDevice,_In_z_ const char* _szFileName);		// �t�@�C�������󂯎��ǂݎ�������Z�b�^�[
	HRESULT SetTexture(_In_ ID3D11Device* d3dDevice,_In_bytecount_(wicDataSize) const uint8_t* wicData,_In_ size_t _wicDataSize);	// �t�@�C�������󂯎��ǂݎ�������Z�b�^�[
	HRESULT SetTexture(ID3D11ShaderResourceView* _pTexture);								// ���ڃV�F�[�_�[���\�[�X�t�@�C�������炤�Z�b�^�[
	ID3D11ShaderResourceView* GetTexture() const;

	//---�ϐ�

private:
	//---�֐�
	//----- �e�N�X�`������������t�@�C�����琶�� -----
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


	//---�ϐ�
	ID3D11ShaderResourceView* pTexture;	// �`��Ɏg���e�N�X�`��

};