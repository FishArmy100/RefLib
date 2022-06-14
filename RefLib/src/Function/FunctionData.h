#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"
#include "Instance/Instance.h"
#include "Argument/Argument.h"
#include "Misc/ParameterData.h"
#include <array>
#include "Attributes/AttributeHolder.h"

namespace RefLib
{
	struct FunctionData
	{
		template<typename TReturn, typename... TArgs>
		FunctionData(const std::string& name, TReturn(*func)(TArgs...), const std::vector<Type>& templateParams, const std::vector<std::string>& paramNames, const std::vector<Variant>& attributes) :
			Name(name), ReturnType(Type::Get<TReturn>()), TemplateParams(templateParams), 
			SignatureType(Type::Get<decltype(func)>()), Parameters({}),
			Attributes(std::make_shared<AttributeHolder>(attributes))
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				std::array<Type, sizeof...(TArgs)> paramTypes = { Type::Get<TArgs>()... };
				for (int i = 0; i < paramTypes.size(); i++)
				{
					if (i < paramNames.size())
						Parameters.push_back(ParameterData(paramNames[i], i, paramTypes[i]));
					else
						Parameters.push_back(ParameterData(i, paramTypes[i]));
				}
			}

			CallFunc = [=](std::vector<Argument> args, std::vector<ParameterData>& params) -> Variant
			{
				if (Utils::ArgListCanCallParamData(params, args))
				{
					return Call(args, func, std::make_index_sequence<sizeof...(TArgs)>{});
				}
			};
		}

		bool IsTemplated() { return TemplateParams.size() > 0; }

		std::string Name;
		Type ReturnType;
		std::vector<ParameterData> Parameters;
		std::vector<Type> TemplateParams;
		Type SignatureType;
		std::shared_ptr<AttributeHolder> Attributes; 

		std::function<Variant(std::vector<Argument>, std::vector<ParameterData>&)> CallFunc;

	private:
		template<typename TReturn, typename... TArgs, size_t... indexes>
		Variant Call(std::vector<Argument> args, TReturn(*func)(TArgs...), std::index_sequence<indexes...>)
		{
			if constexpr (std::is_void_v<TReturn>)
			{
				func(args[indexes].GetValue<TArgs>()...);
				return Variant::GetVoidVarient();
			}
			else
			{
				return func(args[indexes].GetValue<TArgs>()...);
			}
		}
	};
}

