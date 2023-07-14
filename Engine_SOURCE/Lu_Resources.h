#pragma once
#include "Lu_Resource.h"

namespace Lu
{
	class Resources
	{
	private:
		static std::map<std::wstring, std::shared_ptr<Resource>> m_Resources;

	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& _Key)
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = m_Resources.find(_Key);

			if (iter != m_Resources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& _Key, const std::wstring& _Path)
		{
			// 키값으로 탐색
			std::shared_ptr<T> resource = Resources::Find<T>(_Key);
			if (resource != nullptr)
			{
				return resource;
			}

			// 해당 리소스가 없다면
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(_Path)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(_Key);
			resource->SetPath(_Path);
			m_Resources.insert(std::make_pair(_Key, resource));

			return std::dynamic_pointer_cast<T>(resource);
		}

		template <typename T>
		static void Insert(const std::wstring& _Key, std::shared_ptr<T> _Resource)
		{
			m_Resources.insert(std::make_pair(_Key, _Resource));
		}
	};
}

