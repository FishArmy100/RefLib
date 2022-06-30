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
#include "Attributes/AttributeHolder.h"

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
		MethodData(const std::string& name, TReturn(TClass::* method)(TArgs...), const std::vector<Type>& templateParams, AccessLevel level, const std::vector<std::string>& paramNames, const std::vector<Variant>& attributes) :
			MethodData(name, method, level, paramNames, attributes)
		{
			TemplateParams = templateParams;
			IsTemplate = true;
		}

		template<typename TClass, typename TReturn, typename... TArgs>
		MethodData(const std::string& name, TReturn(TClass::* method)(TArgs...), AccessLevel level, const std::vector<std::string>& paramNames, const std::vector<Variant>& attributes) :
			Name(name), ReturnType(Type::Get<TReturn>()), 
			DeclaringType(Type::Get<TClass>()),
			SignatureType(Type::Get<decltype(method)>()),
			Level(level), Attributes(std::make_shared<AttributeHolder>(attributes)),
			IsTemplate(false)
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

				if (ref.GetType().GetId() == Type::Get<TClass>().GetId() || ref.GetType().IsDerivedFrom(Type::Get<TClass>()))
				{
					TClass* obj = (TClass*)ref.GetRawData();
					return std::move(Call(obj, method, args, std::make_integer_sequence<size_t, sizeof...(TArgs)>{}, Containter<TArgs...>{}));
				}

				return Variant();
			};
		}

		MethodData(const MethodData& data) = default;

		std::string Name;
		Type ReturnType;
		Type DeclaringType;
		std::vector<ParameterData> Parameters;
		std::vector<Type> TemplateParams;
		Type SignatureType;
		AccessLevel Level;
		std::shared_ptr<AttributeHolder> Attributes;
		bool IsTemplate;

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
				return std::move(Variant((instance->*method)(((args[ints].GetValue<typename Utils::TypeGetter<ints, TArgs...>::Type>())) ...)));
			}
		}
	};
}

