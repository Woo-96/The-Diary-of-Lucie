#pragma once
#include "Lu_Resource.h"
#include "Lu_Graphics.h"

namespace Lu::graphics
{
	class ComputeShader : public Resource
	{
	public:
		ComputeShader(int X, int Y, int Z);
		ComputeShader();
		virtual ~ComputeShader();

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob>			m_CSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

		UINT m_ThreadGroupCountX;
		UINT m_ThreadGroupCountY;
		UINT m_ThreadGroupCountZ;

		UINT m_GroupX;
		UINT m_GroupY;
		UINT m_GroupZ;

		tMtrlConst	m_Const;

	public:
		bool Create(const std::wstring& _Name, const std::string& _MethodName);
		void OnExcute();

		virtual void Binds() {}
		virtual void Clear() {}

	public:
		virtual HRESULT Load(const std::wstring& _Path) { return S_FALSE; };
	};
}


