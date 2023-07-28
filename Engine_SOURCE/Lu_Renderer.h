#pragma once
#include "Lucie_Engine.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Mesh.h"
#include "Lu_Shader.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Camera.h"

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

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 CameraPosition;
		Vector2 CameraScale;
		Vector2 Resolution;
	};

	CBUFFER(NumberCB, CBSLOT_NUMBER)
	{
		Vector2 vLtUV;
		Vector2 vSliceUV;
	};

	CBUFFER(AnimatorCB, CBSLOT_ANIMATION2D)
	{
		Vector2 SpriteLeftTop;
		Vector2 SpriteSize;
		Vector2 SpriteOffset;
		Vector2 AtlasSize;
		UINT	AnimationType;
		UINT	Reverse;
		UINT	Padding[2];
	};

	CBUFFER(ButtonCB, CBSLOT_NUMBER)
	{
		Vector2 vLtUV;
		Vector2 vSliceUV;
	};

	extern Lu::graphics::ConstantBuffer*					constantBuffer[(UINT)eCBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState>			blendStates[];

	extern Lu::Camera*										mainCamera;
	extern std::vector<Lu::Camera*>							cameras;
	extern std::vector<DebugMesh>							debugMeshs;

	void Initialize();
	void Render();
	void Release();

	void PushDebugMeshAttribute(DebugMesh _Mesh);
}
