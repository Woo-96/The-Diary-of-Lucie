#pragma once
#include "Lu_Component.h"
#include "Lu_Mesh.h"
#include "Lu_Material.h"

namespace Lu
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

	private:
		std::shared_ptr<Mesh>		m_Mesh;
		std::shared_ptr<Material>	m_Material;

	public:
		void SetMesh(std::shared_ptr<Mesh> _Mesh)
		{ 
			m_Mesh = _Mesh;
		}

		void SetMaterial(std::shared_ptr<Material> _Material)
		{ 
			m_Material = _Material;
		}

		std::shared_ptr<Material> GetMaterial() 
		{ 
			return m_Material; 
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}
