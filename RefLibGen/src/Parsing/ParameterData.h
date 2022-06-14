#pragma once
#include <string>

namespace RefLibGen
{
	struct ParameterData
	{
		ParameterData(const std::string& name, const std::string& typeName) :
			Name(name), TypeName(typeName) {}

		std::string Name;
		std::string TypeName;
	};
}