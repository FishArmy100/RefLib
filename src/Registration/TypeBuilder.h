#pragma once
#include "Types/Type.h"
#include "Property/PropertyData.h"
#include "Method/MethodData.h"
#include "Types/MemberContainers.h"
#include "Constructor/ConstructorData.h"
#include <memory>
#include "Method/MethodContainer.h"

namespace RefLib
{
	template<typename TClass>
	class TypeBuilder
	{
	public:
		TypeBuilder(std::string_view name) : m_Name(name), m_Properties({}), m_Methods({}) {}

		template<typename TProp>
		void AddProperty(const std::string& name, TProp TClass::* prop, AccessLevel level = AccessLevel::Public) 
		{
			m_Properties.push_back(PropertyData(name, prop, level));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), AccessLevel level = AccessLevel::Public)
		{
			m_Methods.push_back(MethodData(name, method, level));
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
			TypeDataPrototype prototype;
			prototype.Properties = new PropertyContainer(m_Properties);
			prototype.Methods = new MethodContainer(m_Methods);
			prototype.Constructors = new ConstructorContainer(m_Constructors);
			return Type::RegisterType<TClass>(m_Name, prototype);
		}

	private:
		std::string m_Name;
		std::vector<PropertyData> m_Properties;
		std::vector<MethodData> m_Methods;
		std::vector<ConstructorData> m_Constructors;
	};
}

