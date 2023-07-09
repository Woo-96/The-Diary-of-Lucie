#pragma once
#include "Lu_Entity.h"

namespace Lu
{
	class Resource
	{
	public:
		Resource(const enums::eResourceType _Type);
		virtual ~Resource();

	private:
		const enums::eResourceType	m_Type;
		std::wstring				m_Key;
		std::wstring				m_Path;

	public:
		void SetKey(const std::wstring& _Key) 
		{ 
			m_Key = _Key;
		}

		void SetPath(const std::wstring& _Path) 
		{ 
			m_Path = _Path;
		}

	public:
		enums::eResourceType GetType() 
		{ 
			return m_Type; 
		}

		const std::wstring& GetKey() 
		{ 
			return m_Key; 
		}

		const std::wstring& GetPath() 
		{ 
			return m_Path; 
		}

	public:
		virtual HRESULT Load(const std::wstring& _Path) = 0;
	};
}
