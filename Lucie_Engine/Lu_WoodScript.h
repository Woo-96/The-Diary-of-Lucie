#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class PlayerScript;
	class WoodScript : public Script
	{
	public:
		WoodScript();
		virtual ~WoodScript();

	private:
		PlayerScript* m_Player;

	public:
		void SetPlayer(PlayerScript* _Player)
		{
			m_Player = _Player;
		}

	public:
		virtual void OnCollisionStay(Collider2D* _Other);
	};
}


