#pragma once
#include "Lu_ComputeShader.h"

namespace Lu::graphics
{
	class PaintShader : public ComputeShader
	{
	public:
		PaintShader();
		virtual ~PaintShader();

	private:
		std::shared_ptr<class Texture> m_Target;

	public:
		void SetTarget(std::shared_ptr<class Texture> _Target) 
		{
			m_Target = _Target;
		}

	public:
		virtual void Binds() override;
		virtual void Clear() override;
	};
}