#include "Lu_Script.h"

namespace Lu
{
	Script::Script()
		: Component(enums::eComponentType::Script)
	{
		SetName(L"Script");
	}

	Script::~Script()
	{
	}
}