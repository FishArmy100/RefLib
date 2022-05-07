#pragma once
#include "Types/Type.h"
#include "Property/PropertyData.h"
#include "Method/MethodData.h"
#include <memory>

namespace RefLib
{
	template<typename TClass>
	class TypeBuilder
	{
	public:
		TypeBuilder(std::string_view name) : m_Name(name), m_Properties(MemberContainer<PropertyData*>()) {}
		
		template<typename TProp>
		void AddProperty(const std::string& name, TProp TClass::* prop)
		{
			PropertyData* data = new PropertyData(name, prop);
			m_Properties.insert(std::make_pair(name, data));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...))
		{
			MethodData* data = new MethodData(name, method);
			m_Methods.insert(std::make_pair(name, data));
		}

		bool Register()
		{
			return Type::RegisterType<TClass>(m_Name, m_Properties, m_Methods);
		}

	private:
		std::string m_Name;
		MemberContainer<PropertyData*> m_Properties;
		MemberContainer<MethodData*> m_Methods;
	};
}

