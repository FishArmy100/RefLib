#pragma once
#include "Types/Type.h"
#include "Reference/Reference.h"
#include <functional>

namespace RefLib
{
	struct Test
	{
		int x;
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

			m_SetFunc = [=](Type objT, void* objData, Type valT, void* data)
			{
				if (objT == Type::Get<TClass>() && valT == Type::Get<TProp>())
				{
					TClass* castObj = (TClass*)objData;
					TProp* val = (TProp*)data;
					(*castObj).*prop = *val;
					return true;
				}
				else if(objT == Type::Get<TClass>() && (valT == Type::Get<Variant>() || valT == Type::Get<Variant&>()))
				{
					TClass* castObj = (TClass*)objData;
					Variant* val = (Variant*)data;
					auto convData = (*val).TryConvert<TProp>();
					if (!convData.has_value())
						return false;

					(*castObj).*prop = convData.value();

					return true;
				}

				return false;
			};
		}

		Variant Get(Reference obj) 
		{
			return m_GetFunc(obj.GetType(), obj.GetRawData());
		}

		template<typename T>
		bool Set(Reference obj, T&& value)
		{
			return m_SetFunc(obj.GetType(), obj.GetRawData(), Type::Get<T>(), &value);
		}

		Type GetDeclaringType() { return m_DeclaringType; }
		Type GetType() { return m_Type; }
		std::string_view GetName() { return m_Name; }

	private:
		Type m_DeclaringType;
		Type m_Type;
		std::string m_Name;

		std::function<Variant(Type, void*)> m_GetFunc;
		std::function<bool(Type, void*, Type, void*)> m_SetFunc;
	};
}

