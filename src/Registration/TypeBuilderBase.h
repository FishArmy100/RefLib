#pragma once
#include "Types/Type.h"

namespace RefLib
{
	class TypeBuilderBase
	{
	public:
		virtual Type Register() = 0;
		virtual void SetDeclaring(TypeId typeId) = 0;
	};
}