#pragma once
#include "Misc/ParameterData.h"
#include <functional>
#include "Argument/Argument.h"
#include "Instance/Instance.h"
#include "Misc/TemplateUtils.h"
#include <array>
#include <utility>
#include <iostream>
#include "Misc/AccessLevel.h"

namespace RefLib
{
	struct B
	{
		int Method(int) { return 5; }
	};

	class MethodData
	{
	public:
		template<typename TClass, typename TReturn, typename... TArgs>
		MethodData(const std::string& name, TReturn(TClass::* method)(TArgs...), AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {}) :
			Name(name), ReturnType(Type::Get<TReturn>()), 
			DeclaringType(Type::Get<TClass>()),
			SignatureType(Type::Get<decltype(method)>()),
			Level(level) 
		{
			std::vector<ParameterData> parameters;

			if constexpr (sizeof...(TArgs) > 0)
			{
				std::array<Type, sizeof...(TArgs)> types = { Type::Get<TArgs>()... };

				for (int i = 0; i < types.size(); i++)
				{
					if (paramNames.size() > i)
						parameters.push_back(ParameterData(paramNames[i], i, types[i]));
					else
						parameters.push_back(ParameterData(i, types[i]));
				}
			}

			Parameters = parameters;

			CallFunc = [=](Instance ref, std::vector<Argument> args, std::vector<ParameterData>& params)
			{
				if (args.size() != params.size())
					return Variant(); // invalid varient

				for (int i = 0; i < args.size(); i++)
				{
					if (!args[i].GetType().IsConvertableTo(params[i].ParameterType))
						return Variant();
				}

				TClass* obj = ref.TryConvert<TClass>();

				if (obj == nullptr)
					return Variant();

				return Call(obj, method, args, std::make_integer_sequence<size_t, sizeof...(TArgs)>{}, Containter<TArgs...>{});
			};
		}

		MethodData(const MethodData& data) = default;

		std::string Name;
		Type ReturnType;
		Type DeclaringType;
		std::vector<ParameterData> Parameters;
		Type SignatureType;
		AccessLevel Level;

		std::function<Variant(Instance, std::vector<Argument>, std::vector<ParameterData>&)> CallFunc;


	private:
		template<typename... Types>
		struct Containter {};

		template<typename TClass, typename TReturn, size_t... ints, typename... TArgs>
		Variant Call(TClass* instance, TReturn(TClass::* method)(TArgs...), std::vector<Argument>& args, std::integer_sequence<size_t, ints...> int_seq, const Containter<TArgs...>)
		{
			if constexpr (std::is_void_v<TReturn>)
			{
				(instance->*method)(((args[ints].GetValue<typename Utils::TypeGetter<ints, TArgs...>::Type>())) ...);
				return Variant::GetVoidVarient();
			}
			else
			{
				return Variant((instance->*method)(((args[ints].GetValue<typename Utils::TypeGetter<ints, TArgs...>::Type>())) ...));
			}
		}
	};
}

