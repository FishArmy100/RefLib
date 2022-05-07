#pragma once
#include "ParameterData.h"
#include <functional>
#include "Argument/Argument.h"
#include "Reference/Reference.h"
#include "TemplateUtils.h"
#include <array>
#include <utility>
#include <iostream>

namespace RefLib
{
	struct B
	{
		int Method(int) { return 5; }
	};

	struct MethodData
	{
		template<typename TClass, typename TReturn>
		MethodData(const std::string& name, TReturn (TClass::* method)()) :
			Name(name), ReturnType(Type::Get<TReturn>()), 
			Parameters(std::vector<ParameterData>()), DeclaringType(Type::Get<TClass>())
		{
			CallFunc = [=](Reference ref, std::vector<Argument> args)
			{
				if (args.size() != 0 || Parameters.size() != 0)
					return Variant(); // invalid varient

				TClass* obj = ref.TryConvert<TClass>();

				if (obj == nullptr)
					return Variant();

				return Variant(((*obj).*method)());
			};
		}

		template<typename TClass, typename TReturn, typename... TArgs>
		MethodData(const std::string& name, TReturn(TClass::* method)(TArgs...)) :
			Name(name), ReturnType(Type::Get<TReturn>()), 
			DeclaringType(Type::Get<TClass>())
		{
			std::vector<ParameterData> parameters = std::vector<ParameterData>(sizeof...(TArgs));
			std::array<Type, sizeof...(TArgs)> types = { Type::Get<TArgs>()... };

			for (int i = 0; i < parameters.size(); i++)
				parameters[i] = ParameterData(i, types[i]);

			Parameters = parameters;

			CallFunc = [=](Reference ref, std::vector<Argument> args)
			{
				if (args.size() != Parameters.size())
					return Variant(); // invalid varient

				for (int i = 0; i < args.size(); i++)
				{
					if (args[i].GetType() != Parameters[i].ParameterType)
						return Variant();
				}

				TClass* obj = ref.TryConvert<TClass>();

				if (obj == nullptr)
					return Variant();

				return Call(obj, method, args, std::make_integer_sequence<size_t, sizeof...(TArgs)>{}, Containter<TArgs...>{});
			};
		}

		std::string Name;
		Type ReturnType;
		Type DeclaringType;
		std::vector<ParameterData> Parameters;

		std::function<Variant(Reference, std::vector<Argument>)> CallFunc;


	private:
		/*template
		<
			typename TClass, 
			typename TReturn, 
			typename... TArgs, 
			template<typename...> class ArgCont, 
			typename TInt, 
			TInt... ints
		>
		Variant Call(TClass* instance, ArgCont<TArgs...>, TReturn(TClass::* method)(TArgs...), std::vector<Argument> args, std::integer_sequence<TInt, ints...> sequence)
		{
			return Variant(instance->*method((args[ints].Get<Utils::TypeGetter<ints, TArgs>())...));
		}*/

		template<typename... Types>
		struct Containter {};

		template<typename TClass, typename TReturn, size_t... ints, typename... TArgs>
		Variant Call(TClass* instance, TReturn(TClass::* method)(TArgs...), std::vector<Argument>& args, std::integer_sequence<size_t, ints...> int_seq, const Containter<TArgs...>)
		{
			return Variant((instance->*method)(((args[ints].Get<typename Utils::TypeGetter<ints, TArgs...>::Type>())) ...));
		}
	};
}

