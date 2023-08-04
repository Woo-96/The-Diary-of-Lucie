#include "Lu_Renderer.h"
#include "Lu_Resources.h"
#include "Lu_Texture.h"
#include "Lu_Material.h"
#include "Lu_StructedBuffer.h"
#include "Lu_PaintShader.h"
#include "Lu_ParticleShader.h"

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
		pMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		pMesh->CreateIndexBuffer(indexes.data(), indexes.size());
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
	
		//ParticleCB
		//constantBuffer[(UINT)eCBType::Particle] = new ConstantBuffer(eCBType::Particle);
		//constantBuffer[(UINT)eCBType::Particle]->Create(sizeof(ParticleCB));


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

		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Lu::Resources::Insert(L"PaintShader", paintShader);

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
		// 애니메이션 Mtrl
		std::shared_ptr<Shader> animationShader
			= Resources::Find<Shader>(L"SpriteAnimationShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteAnimaionMaterial", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"PlayerAnimation_Mtrl", material);

		material = std::make_shared<Material>();
		material->SetShader(animationShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SnabyAnimation_Mtrl", material);

		std::shared_ptr<Shader> spriteShader = Resources::Find<Shader>(L"SpriteShader");

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"Mouse_Tex", L"..\\Resources\\Texture\\UI\\Mouse.png");
		material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Mouse_Mtrl", material);

		texture = Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");
		material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		Resources::Insert(L"SpriteMaterial", material);


		texture = Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
		//texture = Resources::Find<Texture>(L"PaintTexuture");
		material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteMaterial02", material);


		std::shared_ptr<Shader> debugShader
			= Resources::Find<Shader>(L"DebugShader");

		material = std::make_shared<Material>();
		material->SetShader(debugShader);
		Resources::Insert(L"DebugMaterial", material);

		std::shared_ptr<Shader> particleShader
			= Resources::Find<Shader>(L"ParticleShader");
		material = std::make_shared<Material>();
		material->SetShader(particleShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"ParticleMaterial", material);

#pragma region	Title Scene Resources
		std::shared_ptr<Shader> pShader = Resources::Find<Shader>(L"SpriteShader");

		// 타이틀 백그라운드
		std::shared_ptr<Texture> pTexture = Resources::Load<Texture>(L"TitleBG_Tex", L"..\\Resources\\Texture\\Map\\Title\\TtitleBG.png");
		std::shared_ptr<Material> pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"TitleBG_Mtrl", pMaterial);

		// 루시
		pTexture = Resources::Load<Texture>(L"Lucie_Tex", L"..\\Resources\\Texture\\Map\\Title\\Lucie.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Lucie_Mtrl", pMaterial);

		// 마리 
		pTexture = Resources::Load<Texture>(L"Mary_Tex", L"..\\Resources\\Texture\\Map\\Title\\Mary.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Mary_Mtrl", pMaterial);

		// 아이오네
		pTexture = Resources::Load<Texture>(L"Ione_Tex", L"..\\Resources\\Texture\\Map\\Title\\Ione.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Ione_Mtrl", pMaterial);

		// 타이틀 문구
		pTexture = Resources::Load<Texture>(L"Title_Tex", L"..\\Resources\\Texture\\Map\\Title\\Title.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Title_Mtrl", pMaterial);
#pragma endregion
#pragma region Lobby Scene Resources
		//pShader = Resources::Find<Shader>(L"SpriteShader");

		pTexture = Resources::Load<Texture>(L"Room_Tex", L"..\\Resources\\Texture\\Map\\Lobby\\Room.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"LobbyBG_Mtrl", pMaterial);
#pragma endregion
#pragma region Stage Scene Resources
		//pShader = Resources::Find<Shader>(L"SpriteShader");

		// WeaponChoiceScene
		pTexture = Resources::Load<Texture>(L"WeaponChoice_Tex", L"..\\Resources\\Texture\\Map\\Stage\\WeaponChoice.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"WeaponChoiceBG_Mtrl", pMaterial);

		// DiceScene
		pTexture = Resources::Load<Texture>(L"Dice_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Dice.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"DiceBG_Mtrl", pMaterial);

		// StoreScene
		pTexture = Resources::Load<Texture>(L"Store_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Store.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"StoreBG_Mtrl", pMaterial);

		// Nomal1Scene
		pTexture = Resources::Load<Texture>(L"Nomal1_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Nomal_1.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"Nomal1BG_Mtrl", pMaterial);

		// Nomal2Scene
		pTexture = Resources::Load<Texture>(L"Nomal2_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Nomal_2.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"Nomal2BG_Mtrl", pMaterial);

		// MidBossScene
		pTexture = Resources::Load<Texture>(L"MidBoss_Tex", L"..\\Resources\\Texture\\Map\\Stage\\MidBoss.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"MidBossBG_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"BossHP_Tex", L"..\\Resources\\Texture\\UI\\Boss\\BossHP.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BossHP_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"MidBossFrame_Tex", L"..\\Resources\\Texture\\UI\\Boss\\boss_mid_Frame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MidBossHPFrame_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"MidBossName_Tex", L"..\\Resources\\Texture\\UI\\Boss\\Boss_kingSlime.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MidBossName_Mtrl", pMaterial);

		// BossWayScene
		pTexture = Resources::Load<Texture>(L"BossWay_Tex", L"..\\Resources\\Texture\\Map\\Stage\\BossWay.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BossWayBG_Mtrl", pMaterial);

		// BossScene
		pTexture = Resources::Load<Texture>(L"Boss_Tex", L"..\\Resources\\Texture\\Map\\Stage\\Boss.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"BossBG_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"BossFrame_Tex", L"..\\Resources\\Texture\\UI\\Boss\\boss_Frame.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BossHPFrame_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"BossName_Tex", L"..\\Resources\\Texture\\UI\\Boss\\Boss_Iggdrasil.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BossName_Mtrl", pMaterial);

#pragma endregion
#pragma region GameOver Scene Resources
		pTexture = Resources::Load<Texture>(L"GameOver_Tex", L"..\\Resources\\Texture\\Map\\GameOver\\Game_Over.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"GameOver_Mtrl", pMaterial);
#pragma endregion
#pragma region HUD Resources
		//pShader = Resources::Find<Shader>(L"SpriteShader");

		pTexture = Resources::Load<Texture>(L"Layout_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Layout.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Layout_Mtrl", pMaterial);

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

		pTexture = Resources::Load<Texture>(L"HP_Icon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\HP_Icon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"HP_Icon_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"MP_Icon_Tex", L"..\\Resources\\Texture\\UI\\HUD\\MP_Icon.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"MP_Icon_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"quickItem_Tex", L"..\\Resources\\Texture\\UI\\HUD\\quickItem.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"quickItem_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"weaponSlotA_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotA.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"weaponSlotA_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"weaponSlotB_Tex", L"..\\Resources\\Texture\\UI\\HUD\\weaponSlotB.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"weaponSlotB_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"Gold_A_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Gold_A.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Gold_A_Mtrl", pMaterial);

		pShader = Resources::Find<Shader>(L"NumberShader");
		pTexture = Resources::Load<Texture>(L"Gold_B_Tex", L"..\\Resources\\Texture\\UI\\HUD\\Gold_B.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"Gold_B_Mtrl", pMaterial);

		pTexture = Resources::Load<Texture>(L"LvNumber_Tex", L"..\\Resources\\Texture\\UI\\HUD\\LV_Number.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"LvNumber_Mtrl", pMaterial);
#pragma endregion
#pragma region Inventory Resources
		pShader = Resources::Find<Shader>(L"SpriteShader");

		pTexture = Resources::Load<Texture>(L"InvenBG_Tex", L"..\\Resources\\Texture\\UI\\Inventory\\InvenBG.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		Resources::Insert(L"InvenBG_Mtrl", pMaterial);

		pShader = Resources::Find<Shader>(L"ButtonShader");
		pTexture = Resources::Load<Texture>(L"EquipBtn", L"..\\Resources\\Texture\\UI\\Inventory\\.png");
		pMaterial = std::make_shared<Material>();
		pMaterial->SetShader(pShader);
		pMaterial->SetTexture(pTexture);
		pMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"EquipBtn_Mtrl", pMaterial);
#pragma endregion

		pShader = Resources::Find<Shader>(L"SpriteShader");

		texture = Resources::Load<Texture>(L"BowProjectile_Tex", L"..\\Resources\\Texture\\Player\\BowProjectile.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"BowProjectile_Mtrl", material);

		texture = Resources::Load<Texture>(L"WandProjectile_Tex", L"..\\Resources\\Texture\\Player\\WandProjectile.png");
		material = std::make_shared<Material>();
		material->SetShader(pShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"WandProjectile_Mtrl", material);
	}

	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadTexture();
		LoadMaterial();
	}

	void BindLights()
	{
		std::vector<LightAttribute> lightsAttributes = {};
		for (Light2D* light : lights)
		{
			LightAttribute attribute = light->GetAttribute();
			lightsAttributes.push_back(attribute);
		}

		lightsBuffer->SetData(lightsAttributes.data(), (UINT)lightsAttributes.size());
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
