#pragma once
#include "Types/Type.h"
#include "Property/PropertyData.h"
#include "Method/MethodData.h"
#include "Types/MemberContainers.h"
#include "Constructor/ConstructorData.h"
#include <memory>

namespace RefLib
{
	template<typename TClass>
	class TypeBuilder
	{
	public:
		TypeBuilder(std::string_view name) : m_Name(name), m_Properties({}), m_Methods({}) {}

		template<typename TProp>
		void AddProperty(const std::string& name, TProp TClass::* prop)
		{
			m_Properties.push_back(PropertyData(name, prop));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...))
		{
			m_Methods.push_back(MethodData(name, method));
		}

		template<typename... TArgs>
		void AddConstructor(AccessLevel level = AccessLevel::Public) 
		{
			TClass(*ctor)(TArgs...) = [](TArgs... args) { return TClass(args...); };
			TClass*(*ptrCtor)(TArgs...) = [](TArgs... args) { return new TClass(args...); };
			m_Constructors.push_back(ConstructorData(ctor, ptrCtor, level)); 
		}

		bool Register()
		{
			return Type::RegisterType<TClass>(m_Name, new PropertyContainer(m_Properties), new MethodContainer(m_Methods), new std::vector<ConstructorData>(m_Constructors));
		}

	private:
		std::string m_Name;
		std::vector<PropertyData> m_Properties;
		std::vector<MethodData> m_Methods;
		std::vector<ConstructorData> m_Constructors;
	};
}

