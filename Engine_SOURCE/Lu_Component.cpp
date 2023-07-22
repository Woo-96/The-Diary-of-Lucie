#include "Lu_Component.h"


namespace Lu
{
	Component::Component(eComponentType _Type)
		: m_Type(_Type)
		, m_Owner(nullptr)
	{

	}

	Component::~Component()
	{

	}
}