#pragma once
#include "Types/Type.h"
#include "Misc/ParameterData.h"
#include "Misc/AccessLevel.h"
#include "Argument/Argument.h"
#include <array>
#include <functional>
#include "Attributes/AttributeHolder.h"

namespace RefLib
{
	struct ConstructorData
	{
		template<typename TClass, typename... TArgs>
		ConstructorData(TClass(*ctor)(TArgs...), TClass*(*ptrCtor)(TArgs...), AccessLevel level, const std::vector<std::string>& paramNames, const std::vector<Variant>& attributes) :
			InstatiatedType(Type::Get<TClass>()),
			Parameters({}), Level(level),
			Attributes(std::make_shared<AttributeHolder>(attributes))
		{
			std::vector<Type> types = std::vector<Type>{ Type::Get<TArgs>()... };
			for (int i = 0; i < types.size(); i++)
			{
				if (paramNames.size() > i)
					Parameters.push_back(ParameterData(paramNames[i], i, types[i]));
				else
					Parameters.push_back(ParameterData(i, types[i]));
			}

			ConstructorFunc = [=](std::vector<Argument> args) -> Variant
			{
				if constexpr (sizeof...(TArgs) > 0)
				{
					std::array<Type, sizeof...(TArgs)> params = { Type::Get<TArgs>()... };
					if (params.size() != args.size())
						return Variant();


					for (int i = 0; i < params.size(); i++)
					{
						if (!args[i].GetType().IsConvertableTo(params[i]))
							return Variant(); // invalid
					}
				}
				else
				{
					if (args.size() > 0)
						return Variant();
				}

				Container<TArgs...> container;
				return Variant(Construct(ctor, args, container, std::make_index_sequence<sizeof...(TArgs)>{}));
			};

			PtrConstructorFunc = [=](std::vector<Argument> args) -> Variant
			{
				if constexpr (sizeof...(TArgs) > 0)
				{
					std::array<Type, sizeof...(TArgs)> params = { Type::Get<TArgs>()... };
					if (params.size() != args.size())
						return Variant();

					for (int i = 0; i < params.size(); i++)
					{
						if (!args[i].GetType().IsConvertableTo(params[i]))
							return Variant(); // invalid
					}
				}
				else
				{
					if (args.size() > 0)
						return Variant();
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
		std::shared_ptr<AttributeHolder> Attributes;
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

