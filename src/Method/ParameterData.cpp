#include "ParameterData.h"

namespace RefLib
{
	bool Utils::ArgListCanCallParamData(const ParameterList& params, const std::vector<Argument>& args)
	{
		for (int i = 0; i < params.size(); i++)
		{
			if (!args[i].GetType().IsConvertableTo(params[i].ParameterType));
			return false;
		}

		return true;
	}

	bool Utils::TypeListCanCallParamData(const ParameterList& params, const std::vector<Type>& types)
	{
		for (int i = 0; i < params.size(); i++)
		{
			if (!types[i].IsConvertableTo(params[i].ParameterType));
			return false;
		}

		return true;
	}
}

