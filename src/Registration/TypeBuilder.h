#pragma once
#include "Types/Type.h"
#include "Property/PropertyData.h"
#include "Method/MethodData.h"
#include "Constructor/ConstructorData.h"
#include <memory>
#include "Method/MethodContainer.h"
#include "Property/PropertyContainer.h"
#include "Constructor/ConstructorContainer.h"
#include "Types/BaseTypeContainer.h"
#include "Types/NestedTypeContainer.h"
#include <type_traits>
#include "EnumBuilder.h"

namespace RefLib
{
	template<typename TClass>
	class TypeBuilder : public TypeBuilderBase
	{
	public:
		TypeBuilder(std::string_view name) : 
			m_Name(name), m_Properties({}), m_Methods({}),
			m_Constructors({}), m_BaseTypes({}), 
			m_DeclaringType(std::optional<TypeId>())
		{}

		template<typename TProp>
		void AddProperty(const std::string& name, TProp TClass::* prop, AccessLevel level = AccessLevel::Public) 
		{
			m_Properties.push_back(PropertyData(name, prop, level));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {})
		{
			m_Methods.push_back(MethodData(name, method, level, paramNames));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...) const, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {})
		{
			AddMethod(name, reinterpret_cast<TReturn(TClass::*)(TArgs...)>(method), level, paramNames);
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), const std::vector<std::string>& paramNames)
		{
			m_Methods.push_back(MethodData(name, method, AccessLevel::Public, paramNames));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...) const, const std::vector<std::string>& paramNames)
		{
			AddMethod(name, reinterpret_cast<TReturn(TClass::*)(TArgs...)>(method), AccessLevel::Public, paramNames);
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), const std::vector<TypeId>& templateArgs, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {})
		{
			m_Methods.push_back(MethodData(name, method, templateArgs, level, paramNames));
		}

		template<typename TReturn, typename... TArgs>
		void AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...) const, const std::vector<TypeId>& templateArgs, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {})
		{
			AddMethod(name, reinterpret_cast<TReturn(TClass::*)(TArgs...)>(method), templateArgs, level, paramNames);
		}

		template<typename... TArgs>
		void AddConstructor(AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {})
		{
			TClass(*ctor)(TArgs...) = [](TArgs... args) { return TClass(args...); };
			TClass*(*ptrCtor)(TArgs...) = [](TArgs... args) { return new TClass(args...); };
			m_Constructors.push_back(ConstructorData(ctor, ptrCtor, level, paramNames));
		}

		template<typename... TArgs>
		void AddConstructor(const std::vector<std::string>& paramNames)
		{
			AddConstructor<TArgs...>(AccessLevel::Public, paramNames);
		}

		template<typename TBase>
		void AddBaseType(AccessLevel level = AccessLevel::Public)
		{
			static_assert(std::is_base_of_v<TBase, TClass>, "TBase must be a base of TClass");
			m_BaseTypes.push_back(BaseType(Type::Get<TBase>(), level));
		}

		template<typename T>
		std::enable_if_t<!std::is_enum_v<T>, Ref<TypeBuilder<T>>> AddNestedType(const std::string& name)
		{
			std::string typeName = m_Name + "::" + name;
			std::unique_ptr<TypeBuilderBase> builderPtr = std::unique_ptr<TypeBuilder<T>>(new TypeBuilder<T>(typeName));
			m_NestedTypes.emplace_back(std::move(builderPtr));
			return Ref<TypeBuilder<T>>((TypeBuilder<T>*)m_NestedTypes.at(m_NestedTypes.size() - 1).get());
		}

		template<typename T>
		std::enable_if_t<std::is_enum_v<T>, Ref<EnumBuilder<T>>> AddNestedType(const std::string& name)
		{
			std::string typeName = m_Name + "::" + name;
			std::unique_ptr<TypeBuilderBase> builderPtr = std::unique_ptr<EnumBuilder<T>>(new EnumBuilder<T>(typeName, {}));
			m_NestedTypes.emplace_back(std::move(builderPtr));
			return Ref<EnumBuilder<T>>((EnumBuilder<T>*)m_NestedTypes.at(m_NestedTypes.size() - 1).get());
		}

		Type Register() override
		{
			std::vector<Type> nestedTypes;
			TypeId id = Type::Get<TClass>().GetId();
			for (auto& typeRegister : m_NestedTypes)
			{
				typeRegister->SetDeclaring(id);
				Type t = typeRegister->Register(); 
				nestedTypes.push_back(t);
			}

			TypeDataPrototype prototype;
			prototype.Properties = new PropertyContainer(m_Properties);
			prototype.Methods = new MethodContainer(m_Methods);
			prototype.Constructors = new ConstructorContainer(m_Constructors);
			prototype.BaseTypes = new BaseTypeContainer(m_BaseTypes);
			prototype.NestedTypes = new NestedTypeContainer(nestedTypes);
			return Type::RegisterType<TClass>(m_Name, prototype, m_DeclaringType);
		}

	protected:
		void SetDeclaring(TypeId id) override
		{
			m_DeclaringType = id;
		}

	private:
		std::optional<TypeId> m_DeclaringType;

		std::string m_Name;
		std::vector<PropertyData> m_Properties;
		std::vector<MethodData> m_Methods;
		std::vector<ConstructorData> m_Constructors;
		std::vector<BaseType> m_BaseTypes;
		std::vector<std::unique_ptr<TypeBuilderBase>> m_NestedTypes; 
	};
}

