#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"

namespace RefLib
{
	struct ParameterData
	{
		ParameterData(const std::string& name, int index, Type type, Variant defaultValue = Variant()) :
			Name(name), Index(index), ParameterType(type), DefaultValue(defaultValue)
		{

		}

		ParameterData(int index, Type type) :
			Index(index), ParameterType(type), 
			Name("Parameter_" + std::to_string(index)),
			DefaultValue(Variant())
		{

		}

		ParameterData() : 
			Name(""), Index(-1), 
			ParameterType(Type::Invalid()), 
			DefaultValue(Variant())
		{
			
		}

		ParameterData(const ParameterData&) = default;
		~ParameterData() = default;

		bool IsValid() { return Index != -1; }

		std::string Name;
		int Index;
		Type ParameterType;
		Variant DefaultValue;
	};
}