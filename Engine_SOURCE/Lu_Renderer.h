#pragma once
#include "Lucie_Engine.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Mesh.h"
#include "Lu_Shader.h"
#include "Lu_ConstantBuffer.h"

using namespace Lu::math;
using namespace Lu::graphics;
namespace renderer
{
	struct Vertex
	{
		Vector3 vPos;
		Vector4 vColor;
		Vector2 vUV;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix mWorld;
		Matrix mView;
		Matrix mProjection;
	};

	extern Vertex							m_arrVertex[];
	extern Lu::graphics::ConstantBuffer*	m_ConstantBuffer[(UINT)eCBType::End];


	void Initialize();
	void Release();
}
