#include "Type.h"

namespace RefLib
{
	std::vector<TypeData*> Type::s_TypeDatas{};

	bool Type::IsAssignableFrom(Type t)
	{
		if (this->IsConst())
			return false;
	}
}