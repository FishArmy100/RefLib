#include "Method.h"

namespace RefLib
{
	Variant Method::Invoke(std::vector<Argument> args)
	{
		if (this->GetDeclaringType().IsDefaultConstructable())
		{
			Variant v = this->GetDeclaringType().Create({});
			return Invoke(v, args);
		}

		return Variant();
	}
}