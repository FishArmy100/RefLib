#pragma once
#include "Types/Type.h"
#include "Instance/Instance.h"
#include <functional>
#include "Argument/Argument.h"
#include "Misc/AccessLevel.h"
#include "Attributes/AttributeHolder.h"

namespace RefLib
{
	struct A
	{
		const int a;
	};

	struct PropertyData
	{
		template<typename TClass, typename TProp>
		PropertyData(std::string_view name, TProp TClass::* prop, AccessLevel level, const std::vector<Variant>& attributes)
			: DeclaringType(Type::Get<TClass>()), PropType(Type::Get<TProp>()), Name(name), 
			  Level(level), Attributes(std::make_shared<AttributeHolder>(attributes))
		{
			GetFunc = [=](Instance instance) -> Variant
			{
				if (instance.GetType().GetId() == Type::Get<TClass>().GetId() || instance.GetType().IsDerivedFrom(Type::Get<TClass>()))
				{
					TClass* castObj = (TClass*)instance.GetRawData();
					TProp& data = *(castObj).*prop;
					return Variant(data);
				}

				return Variant(); // invalid varient
			};

			SetFunc = [=](Instance instance, Argument arg)
			{
				using SetableType = std::remove_cv_t<std::remove_reference_t<TProp>>&;

				if (instance.GetType().GetId() != Type::Get<TClass>().GetId() && !instance.GetType().IsDerivedFrom(Type::Get<TClass>()))
					return false;

				if (Type::Get<TProp>().IsAssignableFrom(arg.GetType()))
				{
					TClass* castObj = (TClass*)instance.GetRawData();
					const_cast<SetableType>((*castObj).*prop) = arg.GetValue<TProp>(); // the const_cast is mostly to remove compiler erros, doesnt actually do anything
					return true;
				}
				else if(arg.IsVarient())
				{
					TClass* castObj = (TClass*)instance.GetRawData();
					Variant& var = arg.GetValue<Variant>();
					auto convData = var.TryConvert<TProp>();
					if (!convData.has_value())
						return false;

					if (Type::Get<TProp>().IsConst())
						return false;

				 	const_cast<SetableType>((*castObj).*prop) = convData.value(); // the const_cast is mostly to remove compiler erros, doesnt actually do anything

					return true;
				}

				return false;
			};
		}

		Type DeclaringType;
		Type PropType;
		std::string Name;

		AccessLevel Level;
		std::shared_ptr<AttributeHolder> Attributes;

		std::function<Variant(Instance)> GetFunc;
		std::function<bool(Instance, Argument)> SetFunc;
	};
}

