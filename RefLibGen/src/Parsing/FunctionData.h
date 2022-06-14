#pragma once
#include "ParameterData.h"
#include <vector>

namespace RefLibGen
{
	struct FunctionData
	{
		FunctionData(const std::string& name, const std::vector<ParameterData>& params) :
			Name(name), Parameters(params) {}

		std::string Name;
		std::vector<ParameterData> Parameters;
	};
}