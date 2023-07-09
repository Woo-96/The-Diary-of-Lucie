#pragma once
#include "Lu_Graphics.h"

namespace Lu::graphics
{
	class ConstantBuffer : public GpuBuffer
	{
	public:
		ConstantBuffer(const eCBType type);
		~ConstantBuffer();

	private:
		const eCBType m_Type;

	public:
		bool Create(size_t size);
		void SetData(void* data);
		void Bind(eShaderStage stage);
	};
}
