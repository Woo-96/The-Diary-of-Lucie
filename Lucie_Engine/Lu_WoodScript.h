#pragma once
#include "Lu_Script.h"

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
		virtual void OnCollisionEnter(Collider2D* _Other);
		virtual void OnCollisionExit(Collider2D* _Other);
	};
}


