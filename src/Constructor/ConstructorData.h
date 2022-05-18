#pragma once
#include "Types/Type.h"
#include "Method/ParameterData.h"
#include "AccessLevel.h"
#include "Argument/Argument.h"
#include <array>
#include <functional>

namespace RefLib
{
	struct ConstructorData
	{
		template<typename TClass, typename... TArgs>
		ConstructorData(TClass(*ctor)(TArgs...), TClass*(*ptrCtor)(TArgs...), AccessLevel level, const std::vector<ParameterData>& parameters = {}) :
			InstatiatedType(Type::Get<TClass>()),
			Parameters(parameters),
			Level(level)
		{
			ConstructorFunc = [=](std::vector<Argument> args) -> Variant
			{
				std::array<Type, sizeof...(TArgs)> params = { Type::Get<TArgs>()... };
				if (params.size() != args.size())
					return Variant(); 

				for (int i = 0; i < params.size(); i++)
				{
					if (!args[i].GetType().IsConvertableTo(params[i]))
						return Variant(); // invalid
				}

				Container<TArgs...> container;
				return Variant(Construct(ctor, args, container, std::make_index_sequence<sizeof...(TArgs)>{}));
			};

			PtrConstructorFunc = [=](std::vector<Argument> args) -> Variant
			{
				std::array<Type, sizeof...(TArgs)> params = { Type::Get<TArgs>()... };
				if (params.size() != args.size())
					return Variant();

				for (int i = 0; i < params.size(); i++)
				{
					if (!args[i].GetType().IsConvertableTo(params[i]))
						return Variant(); // invalid
				}

				Container<TArgs...> container;
				return Variant(ConstructPtr(ptrCtor, args, container, std::make_index_sequence<sizeof...(TArgs)>{}));
			};
		}

		ConstructorData(const ConstructorData& other) = default;
		~ConstructorData() = default;

		Type InstatiatedType;
		std::vector<ParameterData> Parameters;
		AccessLevel Level;
		std::function<Variant(std::vector<Argument>)> ConstructorFunc;
		std::function<Variant(std::vector<Argument>)> PtrConstructorFunc;

	private:
		template<typename... Types>
		struct Container {};

		template<typename TClass, typename... TArgs, size_t... I>
		TClass Construct(TClass(*ctor)(TArgs...), std::vector<Argument> args, Container<TArgs...>, std::index_sequence<I...>)
		{
			return ctor(args[I].GetValue<TArgs>()...);
		}

		template<typename TClass, typename... TArgs, size_t... I>
		TClass* ConstructPtr(TClass*(*ctor)(TArgs...), std::vector<Argument> args, Container<TArgs...>, std::index_sequence<I...>)
		{
			return ctor(args[I].GetValue<TArgs>()...);
		}
	};
}

