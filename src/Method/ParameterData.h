#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"
#include "Argument/Argument.h"

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

		ParameterData(const ParameterData&) = default;
		~ParameterData() = default;

		std::string Name;
		int Index;
		Type ParameterType;
		Variant DefaultValue;
	};

	using ParameterList = std::vector<ParameterData>;

	namespace Utils
	{
		bool ArgListCanCallParamData(const ParameterList& params, const std::vector<Argument>& args);
		bool TypeListCanCallParamData(const ParameterList& params, const std::vector<Type>& types);
	}
}