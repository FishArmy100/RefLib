#pragma once
#include "Types/Type.h"
#include "Instance/Instance.h"
#include <functional>
#include "Argument/Argument.h"
#include "AccessLevel.h"

namespace RefLib
{
	struct A
	{
		const int a;
	};

	class PropertyData
	{
	public:
		PropertyData() : m_Name(""), m_DeclaringType(Type::Invalid()), m_Type(Type::Invalid()), m_AccessLevel(AccessLevel::Private)
		{}

		template<typename TClass, typename TProp>
		PropertyData(std::string_view name, TProp TClass::* prop, AccessLevel level = AccessLevel::Public)
			: m_DeclaringType(Type::Get<TClass>()), m_Type(Type::Get<TProp>()), m_Name(name), 
			  m_AccessLevel(level)
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
					const_cast<SetableType>((*castObj).*prop) = arg.GetValue<TProp>(); // the const_cast is mostly to remove compiler erros, dont actually do anything
					return true;
				}
				else if(arg.IsVarient())
				{
					TClass* castObj = (TClass*)objData;
					Variant& var = arg.GetValue<Variant>();
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

		Variant Get(Instance obj) 
		{
			return m_GetFunc(obj.GetType(), obj.GetRawData());
		}


		bool Set(Instance obj, Argument arg)
		{
			return m_SetFunc(obj.GetType(), obj.GetRawData(), arg);
		}

		bool IsValid() { return m_Type != Type::Invalid(); }

		Type GetDeclaringType() const { return m_DeclaringType; }
		Type GetType() const { return m_Type; }
		std::string_view GetName() const { return m_Name; }
		AccessLevel GetAccessLevel() const { return m_AccessLevel; }

	private:
		Type m_DeclaringType;
		Type m_Type;
		std::string m_Name;

		AccessLevel m_AccessLevel;

		std::function<Variant(Type, void*)> m_GetFunc;
		std::function<bool(Type, void*, Argument)> m_SetFunc;
	};
}

