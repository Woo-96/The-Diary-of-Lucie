#pragma once
#include "Lu_Entity.h"
#include "Lu_Graphics.h"

namespace Lu::graphics
{
	class StructedBuffer : public GpuBuffer
	{
	public:
		StructedBuffer();
		virtual ~StructedBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ReadBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_WriteBuffer;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_SRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	m_UAV;
		eViewType	m_Type;

		UINT		m_Size;
		UINT		m_Stride;

		UINT		m_SRVSlot;
		UINT		m_UAVslot;

	public:
		UINT GetSize() const
		{ 
			return m_Size;
		}

		UINT GetStride() const
		{ 
			return m_Stride; 
		}

	public:
		bool Create(UINT _Size, UINT _Stride, eViewType _Type, void* _Data, bool _bCPUAccess = false);
		bool CreateRWBuffer();
		void SetData(void* _Data, UINT _Stride);
		void GetData(void* _Data, UINT _Size);
		void BindSRV(eShaderStage _Stage, UINT _Slot);
		void BindUAV(UINT _Slot);

		void Clear();
	};
}
