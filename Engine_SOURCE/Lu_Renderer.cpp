#include "Lu_Renderer.h"
#include "Lu_Resources.h"
#include "Lu_Texture.h"
#include "Lu_Material.h"
#include "Lu_StructedBuffer.h"
#include "Lu_PaintShader.h"
#include "Lu_ParticleShader.h"
#include "Lu_AudioClip.h"

namespace renderer
{
	using namespace Lu;
	using namespace Lu::graphics;

	Lu::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};

	// light
	std::vector<Light2D*> lights = {};
	StructedBuffer* lightsBuffer = nullptr;

	Lu::Camera* mainCamera = nullptr;
	std::vector<Lu::Camera*> cameras = {};	
	std::vector<DebugMesh> debugMeshs = {};

	void SetupState()
	{
#pragma region InputLayout
		D3D11_INPUT_ELEMENT_DESC arrLayout[3] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;

		std::shared_ptr<Shader> shader;

		shader = Lu::Resources::Find<Shader>(L"SpriteShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"DebugShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"NumberShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"ProgressBarShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"NumberShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"SpriteAnimationShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"ParticleShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

#pragma endregion
#pragma region Sampler State
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 범위를 벗어나면 텍스쳐가 반복
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSampler(&desc, samplerState[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 0, samplerState[(UINT)eSamplerType::Point].GetAddressOf());

		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSampler(&desc, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 1, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

		//Less
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		//Greater
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		//No Write
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		//None
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		D3D11_BLEND_DESC blendDesc = {};

		//default
		blendStates[(UINT)eBSType::Default] = nullptr;

		// Alpha Blend
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		// one one
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::OneOne].GetAddressOf());
#pragma endregion
	}

	void LoadMesh()
	{
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};

		// ================
		// == Point Mesh ==
		// ================
		Vertex vertex = {};
		vertex.vPos = Vector3(0.0f, 0.0f, 0.0f);
		vertexes.push_back(vertex);
		indexes.push_back(0);
		std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
		pMesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		pMesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
		Resources::Insert(L"PointMesh", pMesh);

		// ===============
		// == Rect Mesh ==
		// ===============
		vertexes.clear();
		indexes.clear();

		vertexes.resize(4);
		vertexes[0].vPos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].vUV = Vector2(0.0f, 0.0f);

		vertexes[1].vPos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].vUV = Vector2(1.0f, 0.0f);

		vertexes[2].vPos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].vColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].vUV = Vector2(1.0f, 1.0f);

		vertexes[3].vPos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].vColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].vUV = Vector2(0.0f, 1.0f);

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// =====================
		// == Debug Rect Mesh ==
		// =====================
		indexes.clear();
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		std::shared_ptr<Mesh> rectDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugRect", rectDebug);
		rectDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		rectDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());


		// =================
		// == Circle Mesh ==
		// =================
		vertexes.clear();
		indexes.clear();

		Vertex v;

		// 반지름
		float fRadius = 0.5f;

		// 원을 몇 개의 삼각형으로 나눌 것인지 (잘게 나눌 수록 원에 가까운 형태)
		UINT Slice = 40;
		// 각도
		float fTheta = XM_2PI / (float)Slice;

		// 중심점
		v.vPos = Vector3(0.f, 0.f, 0.f);
		v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vector2(0.5f, 0.5f);
		vertexes.push_back(v);

		// 정점 위치 지정 (반시계 방향)
		for (UINT i = 0; i < Slice; ++i)
		{
			v.vPos = Vector3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);

			// 중심점 좌표는 (0, 0)인데 UV 좌표의 원점은 (0.5, 0.5)
			// 각 정점에서 0.5만큼 이동하면 UV 좌표
			// 단, 윈도우 좌표에서 y는 반대이므로 -y에 0.5 이동
			v.vUV = Vector2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
			vertexes.push_back(v);
		}

		// 인덱스 설정
		for (UINT i = 0; i < Slice - 1; ++i)
		{
			indexes.push_back(0);
			indexes.push_back(i + 2);
			indexes.push_back(i + 1);
		}

		// 마지막 삼각형 인덱스는 수동으로 설정 (정점 재사용)
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(Slice);

		mesh = std::make_shared<Mesh>();
		Resources::Insert(L"CircleMesh", mesh);
		mesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// =======================
		// == Debug Circle Mesh ==
		// =======================
		indexes.clear();
		for (UINT i = 0; i < Slice; ++i)
		{
			indexes.push_back(i + 1);

			// 혹은 반복문을 1부터 시작해서 Slice + 1까지 반복하여 i를 넣어도 가능
		}
		indexes.push_back(1);

		std::shared_ptr<Mesh> circleDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugCircle", circleDebug);
		circleDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		circleDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
	}

	void LoadBuffer()
	{
		// ==================
		// == Const Buffer ==
		// ==================

		// Transform Buffer
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		// Mtrl Const Buffer
		constantBuffer[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffer[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));
	
		// Animator Buffer
		constantBuffer[(UINT)eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
		constantBuffer[(UINT)eCBType::Animator]->Create(sizeof(AnimatorCB));
	
		//GlobalCB
		constantBuffer[(UINT)eCBType::Global] = new ConstantBuffer(eCBType::Global);
		constantBuffer[(UINT)eCBType::Global]->Create(sizeof(GlobalCB));


		// =====================
		// == Structed Buffer ==
		// =====================

		// light structed buffer
		lightsBuffer = new StructedBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 2, eViewType::SRV, nullptr);
	}

	void LoadShader()
	{
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		Lu::Resources::Insert(L"SpriteShader", spriteShader);

		std::shared_ptr<Shader> spriteAniShader = std::make_shared<Shader>();
		spriteAniShader->Create(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
		spriteAniShader->Create(eShaderStage::PS, L"SpriteAnimationPS.hlsl", "main");
		Lu::Resources::Insert(L"SpriteAnimationShader", spriteAniShader);

		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		debugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		debugShader->SetRSState(eRSType::WireframeNone);
		Lu::Resources::Insert(L"DebugShader", debugShader);

		std::shared_ptr<Shader> numberShader = std::make_shared<Shader>();
		numberShader->Create(eShaderStage::VS, L"NumberVS.hlsl", "main");
		numberShader->Create(eShaderStage::PS, L"NumberPS.hlsl", "main");
		Lu::Resources::Insert(L"NumberShader", numberShader);

		std::shared_ptr<Shader> progressBarShader = std::make_shared<Shader>();
		progressBarShader->Create(eShaderStage::VS, L"ProgressBarVS.hlsl", "main");
		progressBarShader->Create(eShaderStage::PS, L"ProgressBarPS.hlsl", "main");
		Lu::Resources::Insert(L"ProgressBarShader", progressBarShader);

		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Lu::Resources::Insert(L"PaintShader", paintShader);

		std::shared_ptr<ParticleShader> psSystemShader = std::make_shared<ParticleShader>();
		psSystemShader->Create(L"ParticleCS.hlsl", "main");
		Lu::Resources::Insert(L"ParticleSystemShader", psSystemShader);

		std::shared_ptr<Shader> paritcleShader = std::make_shared<Shader>();
		paritcleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		paritcleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		paritcleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
		paritcleShader->SetRSState(eRSType::SolidNone);
		paritcleShader->SetDSState(eDSType::NoWrite);
		paritcleShader->SetBSState(eBSType::AlphaBlend);
		paritcleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		Lu::Resources::Insert(L"ParticleShader", paritcleShader);
	}

	void LoadTexture()
	{
		//paint texture
		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		Lu::Resources::Insert(L"PaintTexuture", uavTexture);
	}

	void LoadMaterial()
	{
#pragma region DebugRender Shader
		std::shared_ptr<Shader> debugShader = Resources::Find<Shader>(L"DebugShader");

		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(debugShader);
		Resources::Insert(L"DebugMaterial", material);

#pragma endregion
#pragma region Animation Shader
		std::shared_ptr<Shader> animationShader = Resources::Find<Shader>(L"SpriteAnimationShader");

		// Default
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteAnimaionMaterial", material);

		// Player
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"PlayerAnimation_Mtrl", material);

		// Monster - Snaby
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SnabyAnimation_Mtrl", material);

		// Monster - Slime
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SlimeAnimation_Mtrl", material);

		// Monster - SlimeJumpFX
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SlimeJumpAnimation_Mtrl", material);

		// Monster - Ent
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"EntAnimation_Mtrl", material);

		// Monster - Ent Attack
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ent_Attack_Thorn_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ent_Attack_PoisonBreath_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ent_Attack_WindBreath_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ent_Attack_TomatoBomb_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ent_Attack_Crater_Mtrl", material);


		// Object - Diary
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"DiaryAnimation_Mtrl", material);

		// FX - MagicCircle
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"MagicCircleAnimation_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"MagicCircle2Animation_Mtrl", material);

		// Object - Coin
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"CoinAnimation_Mtrl", material);

		// Object - Statue
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"StatueAnimation_Mtrl", material);

		// FX - Wand Charge
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandCharge_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandFullCharge_Mtrl", material);

		// Skill - IceBall
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"IceBall_Mtrl", material);

		// Skill - FireStrike
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"FireStrike_Mtrl", material);

		// Skill - Cast
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SkillCast_Mtrl", material);


		// Object - Chest
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Chest_Mtrl", material);

		// FX - ChestOpenFX1
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"ChestOpenFX1_Mtrl", material);

		// FX - ChestOpenFX2
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		Resources::Insert(L"ChestOpenFX2_Mtrl", material);

		// FX - Player Appear
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"AppearFX_Mtrl", material);

		// Object - Portal
		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"PortalAnimation_Mtrl", material);


#pragma endregion
#pragma region Particle Shader
		std::shared_ptr<Shader> particleShader = Resources::Find<Shader>(L"ParticleShader");

		std::shared_ptr<Texture> particleTexture = Resources::Load<Texture>(L"ParticleTex", L"..\\Resources\\Texture\\Particle\\Title\\Particles7.png");
		material = std::make_shared<Material>();
		material->SetShader(particleShader);
		material->SetTexture(particleTexture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"ParticleMaterial", material);
#pragma endregion
#pragma region	Sprite Shader
		std::shared_ptr<Shader> pShader = Resources::Find<Shader>(L"SpriteShader");

		// Object - Mouse
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"Mouse_Tex", L"..\\Resources\\Texture\\UI\\Mouse.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Mouse_Mtrl", material);

		// TitleScene BG
		std::shared_ptr<Texture> pTexture = Resources::Load<Texture>(L"TitleBG_Tex", L"..\\Resources\\Texture\\Map\\Title\\TtitleBG.png");
		std::shared_ptr<Material> pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"TitleBG_Mtrl", pMaterial);

		// Title BG (Lucie)
		pTexture = Resources::Load<Texture>(L"Lucie_Tex", L"..\\Resources\\Texture\\Map\\Title\\Lucie.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Lucie_Mtrl", pMaterial);

		// Title BG (Mary)
		pTexture = Resources::Load<Texture>(L"Mary_Tex", L"..\\Resources\\Texture\\Map\\Title\\Mary.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Mary_Mtrl", pMaterial);

		// Title BG (Ione)
		pTexture = Resources::Load<Texture>(L"Ione_Tex", L"..\\Resources\\Texture\\Map\\Title\\Ione.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ione_Mtrl", pMaterial);

		// Title BG
		pTexture = Resources::Load<Texture>(L"Title_Tex", L"..\\Resources\\Texture\\Map\\Title\\Title.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Title_Mtrl", pMaterial);

		// Ending BG
		pTexture = Resources::Load<Texture>(L"Ending_Tex", L"..\\Resources\\Texture\\Map\\Ending\\Ending.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ending_Mtrl", pMaterial);

		// LobbyScene BG
		pTexture = Resources::Load<Texture>(L"Room_Tex", L"..\\Resources\\Texture\\Map\\Lobby\\Room.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"LobbyBG_Mtrl", pMaterial);

		// WeaponChoiceScene BG
		pTexture = Resources::Load<Texture>(L"WeaponChoice_Tex", L"..\\Resources\\Texture\\Map\\Stage\\WeaponChoice.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"WeaponChoiceBG_Mtrl", pMaterial);

		// DiceScene BG
		pTexture = Resources::Load<Texture>(L"Dice_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Dice.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"DiceBG_Mtrl", pMaterial);

		// StoreScene BG
		pTexture = Resources::Load<Texture>(L"Store_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Store.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"StoreBG_Mtrl", pMaterial);

		// Nomal1Scene BG
		pTexture = Resources::Load<Texture>(L"Nomal1_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Nomal_1.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"Nomal1BG_Mtrl", pMaterial);

		// Nomal2Scene BG
		pTexture = Resources::Load<Texture>(L"Nomal2_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Nomal_2.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"Nomal2BG_Mtrl", pMaterial);

		// MidBossScene BG
		pTexture = Resources::Load<Texture>(L"MidBoss_Tex", L"..\\Resources\\Texture\\Map\\Stage\\MidBoss.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"MidBossBG_Mtrl", pMaterial);

		// Mid Boss Frame
		pTexture = Resources::Load<Texture>(L"MidBossFrame_Tex", L"..\\Resources\\Texture\\UI\\Boss\\boss_mid_Frame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MidBossHPFrame_Mtrl", pMaterial);

		// Mid Boss Name
		pTexture = Resources::Load<Texture>(L"MidBossName_Tex", L"..\\Resources\\Texture\\UI\\Boss\\Boss_kingSlime.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MidBossName_Mtrl", pMaterial);

		// BossWayScene BG
		pTexture = Resources::Load<Texture>(L"BossWay_Tex", L"..\\Resources\\Texture\\Map\\Stage\\BossWay.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BossWayBG_Mtrl", pMaterial);

		// BossScene BG
		pTexture = Resources::Load<Texture>(L"Boss_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Boss.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BossBG_Mtrl", pMaterial);

		// Boss Frame
		pTexture = Resources::Load<Texture>(L"BossFrame_Tex", L"..\\Resources\\Texture\\UI\\Boss\\boss_Frame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BossHPFrame_Mtrl", pMaterial);

		// Boss Name
		pTexture = Resources::Load<Texture>(L"BossName_Tex", L"..\\Resources\\Texture\\UI\\Boss\\Boss_Iggdrasil.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BossName_Mtrl", pMaterial);

		// GameOverScene BG
		pTexture = Resources::Load<Texture>(L"GameOver_Tex", L"..\\Resources\\Texture\\Map\\GameOver\\Game_Over.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"GameOver_Mtrl", pMaterial);

		// Projectile - Bow
		texture = Resources::Load<Texture>(L"BowProjectile_Tex", L"..\\Resources\\Texture\\Player\\BowProjectile.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BowProjectile_Mtrl", material);

		// Projectile - Wand
		texture = Resources::Load<Texture>(L"WandProjectile_Tex", L"..\\Resources\\Texture\\Player\\WandProjectile.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandProjectile_Mtrl", material);

		// Projectile - WandChanneling_Small
		texture = Resources::Load<Texture>(L"WandChanneling_A_Tex", L"..\\Resources\\Texture\\Player\\WandChanneling_A.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandChanneling_A_Mtrl", material);

		// Projectile - WandChanneling_Big
		texture = Resources::Load<Texture>(L"WandChanneling_B_Tex", L"..\\Resources\\Texture\\Player\\WandChanneling_B.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandChanneling_B_Mtrl", material);

		// Projectile - Circle
		texture = Resources::Load<Texture>(L"MonsterProjectile_Circle_Tex", L"..\\Resources\\Texture\\Monster\\MonsterProjectile_Circle.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MonsterProjectile_Circle_Mtrl", material);

		// Projectile - Bubble
		texture = Resources::Load<Texture>(L"MonsterProjectile_Bubble_Tex", L"..\\Resources\\Texture\\Monster\\MidBoss\\Bubble.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MonsterProjectile_Bubble_Mtrl", material);

		// Projectile - BossCircle
		texture = Resources::Load<Texture>(L"MonsterProjectile_BossCircle_Tex", L"..\\Resources\\Texture\\Monster\\MidBoss\\SlimeCircle.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MonsterProjectile_SlimeCircle_Mtrl", material);

		// Projectile - Jump
		texture = Resources::Load<Texture>(L"BigSlimeJump_Tex", L"..\\Resources\\Texture\\Monster\\MidBoss\\BigSlimeJump.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BigSlimeJump_Mtrl", material);

		// Item - GreenJelly
		texture = Resources::Load<Texture>(L"GreenJelly_Tex", L"..\\Resources\\Texture\\Item\\GreenJelly.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"GreenJelly_Mtrl", material);

		// Item - Sword
		texture = Resources::Load<Texture>(L"Sword_Tex", L"..\\Resources\\Texture\\Item\\Sword.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Sword_Mtrl", material);

		// Item - Bow
		texture = Resources::Load<Texture>(L"Bow_Tex", L"..\\Resources\\Texture\\Item\\Bow.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Bow_Mtrl", material);

		// Item - Wand
		texture = Resources::Load<Texture>(L"Wand_Tex", L"..\\Resources\\Texture\\Item\\Wand.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Wand_Mtrl", material);

		// UI - Layout
		pTexture = Resources::Load<Texture>(L"Layout_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Layout.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Layout_Mtrl", pMaterial);

		// UI - QuickItem
		pTexture = Resources::Load<Texture>(L"quickItem_Tex", L"..\\Resources\\Texture\\UI\\HUD\\quickItem.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"QuickItem_Mtrl", pMaterial);

		// UI - WeaponSlot
		pTexture = Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"WeaponSlot_Mtrl", pMaterial);

		// UI - Gold_A
		pTexture = Resources::Load<Texture>(L"Gold_A_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Gold_A.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Gold_A_Mtrl", pMaterial);

		// UI - 채널링 레이아웃
		pTexture = Resources::Load<Texture>(L"Shud_A_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Shud_A.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Shud_A_Mtrl", pMaterial);

		// UI - Inventory BG
		pTexture = Resources::Load<Texture>(L"InvenBG_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\InvenBG.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"InvenBG_Mtrl", pMaterial);

		// UI - Inventory Slot Default
		pTexture = Resources::Load<Texture>(L"Slot_Default_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\Slot_Default.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Slot_Default_Mtrl", pMaterial);

		// UI - Inventory Slot Default Equipped
		pTexture = Resources::Load<Texture>(L"Slot_Default_Equipped_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\Slot_Default_Equipped.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Slot_Default_Equipped_Mtrl", pMaterial);

		// UI - Inventory Slot Hovered
		pTexture = Resources::Load<Texture>(L"Slot_Hovered_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\Slot_Hovered.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Slot_Hovered_Mtrl", pMaterial);

		// UI - Inventory Slot Hovered Equipped
		pTexture = Resources::Load<Texture>(L"Slot_Hovered_Equipped_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\Slot_Hovered_Equipped.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Slot_Hovered_Equipped_Mtrl", pMaterial);

		// UI - Inventory Slot Equipped Label
		pTexture = Resources::Load<Texture>(L"Equipped_Label_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\Equipped_Label.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Equipped_Label_Mtrl", pMaterial);

		// UI - Inventory Gold Icon
		pTexture = Resources::Load<Texture>(L"InvenGoldIcon_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\GoldIcon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"InvenGoldIcon_Mtrl", pMaterial);

		// UI - Skill Element
		pTexture = Resources::Load<Texture>(L"CurrentSkill_Fire_Tex", L"..\\Resources\\Texture\\UI\\HUD\\CurrentSkill_Fire.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"CurrentSkill_Fire_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"CurrentSkill_Ice_Tex", L"..\\Resources\\Texture\\UI\\HUD\\CurrentSkill_Ice.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"CurrentSkill_Ice_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"CurrentSkill_None_Tex", L"..\\Resources\\Texture\\UI\\HUD\\CurrentSkill_None.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"CurrentSkill_None_Mtrl", pMaterial);

		// UI - Cur Skill
		pTexture = Resources::Load<Texture>(L"CurrentSkill_Tex", L"..\\Resources\\Texture\\UI\\HUD\\CurrentSkill.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"CurrentSkill_Mtrl", pMaterial);

		// UI - Skill Icon
		pTexture = Resources::Load<Texture>(L"IceBallIcon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\IceBallIcon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"IceBallIcon_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"FireStrikeIcon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\FireStrikeIcon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"FireStrikeIcon_Mtrl", pMaterial);


		// UI - SkillBG
		pTexture = Resources::Load<Texture>(L"SkillBG_Tex", L"..\\Resources\\Texture\\UI\\HUD\\SkillBG.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"SkillBG_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"SkillBG2_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Transparent.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"SkillBG_Transparent_Mtrl", pMaterial);
		
		// UI - SkillBoxFrame1
		pTexture = Resources::Load<Texture>(L"SkillBoxFrame_Tex", L"..\\Resources\\Texture\\UI\\HUD\\SkillBoxFrame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"SkillBoxFrame_Mtrl", pMaterial);

		// UI - SkillBoxFrame2
		pTexture = Resources::Load<Texture>(L"LearnSkillFrame_Tex", L"..\\Resources\\Texture\\UI\\HUD\\LearnSkillFrame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"LearnSkillFrame_Mtrl", pMaterial);

		// UI - Press any key
		pTexture = Resources::Load<Texture>(L"PressKey_Tex", L"..\\Resources\\Texture\\Map\\Title\\PressKey.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"PressKey_Mtrl", pMaterial);

		// UI - DamageFont 용
		pTexture = Resources::Load<Texture>(L"DamageFont_Tex", L"..\\Resources\\Texture\\UI\\DamageFont.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"DamageFont_Mtrl", pMaterial);
		
		// BG - CameraFX 용
		pTexture = Resources::Load<Texture>(L"Drama_Tex", L"..\\Resources\\Texture\\Map\\Drama.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Drama_Mtrl", pMaterial);


#pragma endregion
#pragma region Number Shader
		// LT, Slice를 정하는 Shader
		pShader = Resources::Find<Shader>(L"NumberShader");

		pTexture = Resources::Load<Texture>(L"Gold_B_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Gold_B.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"Gold_B_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"LvNumber_Tex", L"..\\Resources\\Texture\\UI\\HUD\\LV_Number.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"LvNumber_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"HP_Icon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\HP_Icon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"EmptyHeart_Icon_Mtrl", pMaterial);

		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"FullHeart_Icon_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"MP_Icon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\MP_Icon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"EmptyMana_Icon_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"MP_Icon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\MP_Icon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::CutOut);
		Resources::Insert(L"FullMana_Icon_Mtrl", pMaterial);
#pragma endregion
#pragma region ProgressBar Shader
		pShader = Resources::Find<Shader>(L"ProgressBarShader");

		pTexture = Resources::Load<Texture>(L"BossHP_Tex", L"..\\Resources\\Texture\\UI\\Boss\\BossHP.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"KingSlime_BossHP_Mtrl", pMaterial);

		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BigSlime1_BossHP_Mtrl", pMaterial);

		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BigSlime2_BossHP_Mtrl", pMaterial);

		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"Ent_BossHP_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"TP_Meter_Tex", L"..\\Resources\\Texture\\UI\\HUD\\TP_Meter.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"TP_Meter_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"EXP_Meter_Tex", L"..\\Resources\\Texture\\UI\\HUD\\EXP_Meter.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"EXP_Meter_Mtrl", pMaterial);

		// UI - 채널링 완드 게이지
		pTexture = Resources::Load<Texture>(L"Shud_B_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Shud_B.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Shud_B_Mtrl", pMaterial);

		// UI - 채널링 스킬 게이지
		pTexture = Resources::Load<Texture>(L"Shud_C_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Shud_C.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Shud_C_Mtrl", pMaterial);
#pragma endregion
#pragma region Button Shader
		pShader = Resources::Find<Shader>(L"ButtonShader");

		pTexture = Resources::Load<Texture>(L"EquipBtn", L"..\\Resources\\Texture\\UI\\Inventory\\.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"EquipBtn_Mtrl", pMaterial);

#pragma endregion
	}
	
	void LoadSound()
	{
		Resources::Load<AudioClip>(L"ForestBGM", L"..\\Resources\\Sound\\BGM\\ForestBGM.ogg");
		Resources::Load<AudioClip>(L"LobbyBGM", L"..\\Resources\\Sound\\BGM\\LobbyBGM.ogg");
		Resources::Load<AudioClip>(L"MidBossBGM", L"..\\Resources\\Sound\\BGM\\MidBossBGM.ogg");
		Resources::Load<AudioClip>(L"BossWayBGM", L"..\\Resources\\Sound\\BGM\\BossWayBGM.ogg");
		Resources::Load<AudioClip>(L"BossBGM", L"..\\Resources\\Sound\\BGM\\BossBGM.ogg");
		Resources::Load<AudioClip>(L"GameOverBGM", L"..\\Resources\\Sound\\BGM\\GameOverBGM.ogg");
	}

	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadTexture();
		LoadMaterial();
		LoadSound();
	}

	void BindLights()
	{
		std::vector<LightAttribute> lightsAttributes = {};
		for (Light2D* light : lights)
		{
			LightAttribute attribute = light->GetAttribute();
			lightsAttributes.push_back(attribute);
		}

		//lightsBuffer->SetData(lightsAttributes.data(), (UINT)lightsAttributes.size());
		lightsBuffer->BindSRV(eShaderStage::VS, 13);
		lightsBuffer->BindSRV(eShaderStage::PS, 13);
	}

	void PushDebugMeshAttribute(DebugMesh _Mesh)
	{
		debugMeshs.push_back(_Mesh);
	}

	void Render()
	{
		BindLights();

		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras.clear();
		lights.clear();
	}

	void Release()
	{
		for (ConstantBuffer* buff : constantBuffer)
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}

		delete lightsBuffer;
		lightsBuffer = nullptr;
	}
}
