#pragma once
#include "Types/Type.h"
#include "Reference/Reference.h"
#include <functional>
#include "Argument/Argument.h"

namespace RefLib
{
	struct A
	{
		const int a;
	};

	class PropertyData
	{
	public:
		template<typename TClass, typename TProp>
		PropertyData(std::string_view name, TProp TClass::* prop)
			: m_DeclaringType(Type::Get<TClass>()), m_Type(Type::Get<TProp>()), m_Name(name) 
		{
			m_GetFunc = [=](Type t, void* data) -> Variant
			{
				if (t == Type::Get<TClass>())
				{
					TClass* castObj = (TClass*)data;
					TProp& data = *(castObj).*prop;
					return Variant(data);
				}

				return Variant(); // invalid varient
			};

			m_SetFunc = [=](Type objT, void* objData, Argument arg)
			{
				using SetableType = std::remove_cv_t<std::remove_reference_t<TProp>>&;

				if (Type::Get<TProp>().IsAssignableFrom(arg.GetType()))
				{
					TClass* castObj = (TClass*)objData;
					const_cast<SetableType>((*castObj).*prop) = arg.Get<TProp>(); // the const_cast is mostly to remove compiler erros, dont actually do anything
					return true;
				}
				else if(arg.IsVarient())
				{
					TClass* castObj = (TClass*)objData;
					Variant& var = arg.Get<Variant>();
					auto convData = var.TryConvert<TProp>();
					if (!convData.has_value())
						return false;

					if (Type::Get<TProp>().IsConst())
						return false;

				 	const_cast<SetableType>((*castObj).*prop) = convData.value(); // the const_cast is mostly to remove compiler erros, dont actually do anything

					return true;
				}

				return false;
			};
		}

		Variant Get(Reference obj) 
		{
			return m_GetFunc(obj.GetType(), obj.GetRawData());
		}


		bool Set(Reference obj, Argument arg)
		{
			return m_SetFunc(obj.GetType(), obj.GetRawData(), arg);
		}

		Type GetDeclaringType() { return m_DeclaringType; }
		Type GetType() { return m_Type; }
		std::string_view GetName() { return m_Name; }

	private:
		Type m_DeclaringType;
		Type m_Type;
		std::string m_Name;

		std::function<Variant(Type, void*)> m_GetFunc;
		std::function<bool(Type, void*, Argument)> m_SetFunc;
	};
}

