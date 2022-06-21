#pragma once
#include "Types/TypeId.h"
#include <string>

namespace RefLib
{
	class Type;

	struct TemplateParameter
	{
		TemplateParameter(TypeId type, const std::string& name, int index);
		TemplateParameter(const TemplateParameter& other) = default;
		~TemplateParameter() = default;

		Type GetType();

		TypeId ParamType;
		std::string Name;
		int Index;
	};
}

