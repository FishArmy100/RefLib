#pragma once
#include "Types/Type.h"
#include "Property/PropertyData.h"
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

		bool Register()
		{
			return Type::RegisterType<TClass>(m_Name, m_Properties);
		}

	private:
		std::string m_Name;
		MemberContainer<PropertyData*> m_Properties;
	};
}

