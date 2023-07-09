#include "Lu_Renderer.h"
#include "Lu_Resources.h"
#include "Lu_Texture.h"
#include "Lu_Material.h"

namespace renderer
{
	using namespace Lu;
	using namespace Lu::graphics;

	Vertex m_arrVertex[4] = {};
	Lu::graphics::ConstantBuffer* m_ConstantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState[(UINT)eSamplerType::End] = {};

	void SetupState()
	{
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

		std::shared_ptr<Shader> shader = Lu::Resources::Find<Shader>(L"TriangleShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Lu::Resources::Find<Shader>(L"SpriteShader");
		Lu::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		//Sampler State
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 범위를 벗어나면 텍스쳐가 반복
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSampler(&desc, SamplerState[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 0, SamplerState[(UINT)eSamplerType::Point].GetAddressOf());

		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSampler(&desc, SamplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 1, SamplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
	}

	void LoadBuffer()
	{
		// Vertex Buffer
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(m_arrVertex, 4);

		// Index Buffer
		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// Constant Buffer
		m_ConstantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		m_ConstantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));
	}

	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");
		Lu::Resources::Insert(L"TriangleShader", shader);

		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		Lu::Resources::Insert(L"SpriteShader", spriteShader);

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");

			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			Resources::Insert(L"SpriteMaterial", spriteMateiral);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
			std::shared_ptr<Material> spriteMateiral = std::make_shared<Material>();
			spriteMateiral->SetShader(spriteShader);
			spriteMateiral->SetTexture(texture);
			Resources::Insert(L"SpriteMaterial02", spriteMateiral);
		}
	}

	void Initialize()
	{
		m_arrVertex[0].vPos = Vector3(-0.5f, 0.5f, 0.0f);
		m_arrVertex[0].vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		m_arrVertex[0].vUV = Vector2(0.0f, 0.0f);

		m_arrVertex[1].vPos = Vector3(0.5f, 0.5f, 0.0f);
		m_arrVertex[1].vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		m_arrVertex[1].vUV = Vector2(1.0f, 0.0f);

		m_arrVertex[2].vPos = Vector3(0.5f, -0.5f, 0.0f);
		m_arrVertex[2].vColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		m_arrVertex[2].vUV = Vector2(1.0f, 1.0f);

		m_arrVertex[3].vPos = Vector3(-0.5f, -0.5f, 0.0f);
		m_arrVertex[3].vColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_arrVertex[3].vUV = Vector2(0.0f, 1.0f);

		LoadBuffer();
		LoadShader();
		SetupState();

		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
		texture
			= Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");

		texture->BindShader(eShaderStage::PS, 0);
	}

	void Release()
	{
		for (ConstantBuffer* buff : m_ConstantBuffer)
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}
	}
}
