#pragma once
#include "Lucie_Engine.h"

namespace Lu
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

	private:
		std::wstring	m_Name;
		UINT64			m_ID;

	public:
		void SetName(const std::wstring& _Name) 
		{ 
			m_Name = _Name; 
		}

		void SetID(UINT64 _ID) 
		{ 
			m_ID = _ID;
		}

	public:
		std::wstring& GetName()
		{ 
			return m_Name;
		}

		UINT64 GetID() const
		{ 
			return m_ID; 
		}
	};
}
