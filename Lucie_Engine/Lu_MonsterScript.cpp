#include "Lu_MonsterScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Renderer.h"

namespace Lu
{
	MonsterScript::MonsterScript()
		: m_Animator(nullptr)
		, m_MonsterInfo{}
		, m_AnimDir(eAnimDir::Down)
		, m_PrevAnimDir(eAnimDir::None)
	{
		SetName(L"MonsterScript");
	}

	MonsterScript::~MonsterScript()
	{

	}

	void MonsterScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateAnimation();
	}

	void MonsterScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Radius = m_MonsterInfo.DetectRange;
		mesh.Type = enums::eColliderType::Circle;
		mesh.Color = Vector4(0.f, 0.f, 1.f, 1.f);

		renderer::PushDebugMeshAttribute(mesh);
	}
}