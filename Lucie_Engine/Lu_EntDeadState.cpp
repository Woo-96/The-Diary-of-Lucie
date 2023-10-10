#include "Lu_EntDeadState.h"
#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_Renderer.h"
#include "Lu_CameraScript.h"

namespace Lu
{
	EntDeadState::EntDeadState()
	{
		SetName(L"EntDeadStateScript");
		SetStateType(eState::Dead);
	}

	EntDeadState::~EntDeadState()
	{
	}

	void EntDeadState::Enter()
	{
		if (!(GetEntScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);

		CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCam->SetTargetMove(false);
		pMainCam->SetTarget(GetOwner());
		pMainCam->SetOffset(Vector2(0.f, 0.f));
		pMainCam->SetSpeed(1.f);

		object::Destroy(GetEntScript()->GetHPFrame());
		object::Destroy(GetEntScript()->GetHPBar());
	}

	void EntDeadState::Exit()
	{
		CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCam->SetTarget(SceneManager::FindPlayer());
		pMainCam->SetOffset(Vector2(0.f, 0.f));

		object::Destroy(GetEntScript()->GetOwner());
	}
}
