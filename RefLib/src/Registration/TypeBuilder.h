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
#include "Misc/AccessLevel.h"
#include "Types/DerivedTypeContainer.h"

namespace RefLib
{
	template<typename TClass>
	class TypeBuilder : public TypeBuilderBase
	{
	public:
		TypeBuilder(std::string_view name) : 
			m_Name(name), m_Properties({}), m_Methods({}),
			m_Constructors({}), m_BaseTypes({}), 
			m_DeclaringType(std::optional<TypeId>()),
			m_AsContainerFunc()
		{}

		template<typename TProp>
		TypeBuilder& AddProperty(const std::string& name, TProp TClass::* prop, AccessLevel level = AccessLevel::Public, const std::vector<Variant>& attributes = {})
		{
			m_Properties.push_back(PropertyData(name, prop, level, attributes));
			return *this;
		}

		template<typename TReturn, typename... TArgs>
		TypeBuilder& AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), const std::vector<Type>& templateArgs, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {}, const std::vector<Variant>& attributes = {})
		{
			m_Methods.emplace_back(name, method, templateArgs, level, paramNames, attributes);
			return *this;
		}

		template<typename TReturn, typename... TArgs>
		TypeBuilder& AddMethod(const std::string& name, TReturn(TClass::* method)(TArgs...) const, const std::vector<Type>& templateArgs, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {}, const std::vector<Variant>& attributes = {})
		{
			AddMethod(name, reinterpret_cast<TReturn(TClass::*)(TArgs...)>(method), templateArgs, level, paramNames, attributes);
			return *this;
		}

		template<typename... TArgs>
		TypeBuilder& AddConstructor(AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {}, const std::vector<Variant>& attributes = {})
		{
			TClass(*ctor)(TArgs...) = [](TArgs... args) { return TClass(args...); };
			TClass*(*ptrCtor)(TArgs...) = [](TArgs... args) { return new TClass(args...); };
			m_Constructors.push_back(ConstructorData(ctor, ptrCtor, level, paramNames, attributes));
			return *this;
		}

		template<typename TBase>
		TypeBuilder& AddBaseType(AccessLevel level = AccessLevel::Public)
		{
			static_assert(std::is_base_of_v<TBase, TClass>, "TBase must be a base of TClass");
			m_BaseTypes.push_back(BaseType(Type::Get<TBase>(), level));
			Type::RegisterDerivedType<TBase, TClass>();
			return *this;
		}

		template<typename T>
		Ref<TypeBuilder<T>> AddNestedClass(const std::string& name)
		{
			static_assert(!std::is_enum_v<T>, "Type cannot be an enum");
			std::string typeName = m_Name + "::" + name;
			std::unique_ptr<TypeBuilderBase> builderPtr = std::unique_ptr<TypeBuilder<T>>(new TypeBuilder<T>(typeName));
			m_NestedTypes.emplace_back(std::move(builderPtr));
			return Ref<TypeBuilder<T>>((TypeBuilder<T>*)m_NestedTypes.at(m_NestedTypes.size() - 1).get());
		}

		template<typename T>
		Ref<EnumBuilder<T>> AddNestedEnum(const std::string& name)
		{
			static_assert(std::is_enum_v<T>, "Type must be an enum");
			std::string typeName = m_Name + "::" + name;
			std::unique_ptr<TypeBuilderBase> builderPtr = std::unique_ptr<EnumBuilder<T>>(new EnumBuilder<T>(typeName, {}));
			m_NestedTypes.emplace_back(std::move(builderPtr));
			return Ref<EnumBuilder<T>>((EnumBuilder<T>*)m_NestedTypes.at(m_NestedTypes.size() - 1).get());
		}
		
		template<typename T>
		TypeBuilder& AddNestedClass(const std::string& name, void(*func)(TypeBuilder<T>&))
		{
			static_assert(!std::is_enum_v<T>, "Type cannot be an enum");
			auto builder = AddNestedClass<T>(name);
			func(*(builder.Get()));
			return *this;
		}

		template<typename T>
		TypeBuilder& AddNestedEnum(const std::string& name, void(*func)(EnumBuilder<T>&))
		{
			static_assert(std::is_enum_v<T>, "Type must be an enum");
			auto builder = AddNestedEnum<T>(name);
			func(*(builder.Get()));
			return *this;
		}
		
		template<typename T>
		TypeBuilder& AddAttribute(const T& value)
		{
			m_Attributes.push_back(value);
			return *this;
		}

		TypeBuilder& SetAsContainer(ContainerView(*func)(TClass&))
		{
			m_AsContainerFunc = [=](Instance i) {return func(*i.TryConvert<TClass>()); };
			return *this;
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
			prototype.DerivedTypes = new DerivedTypeContainer({});
			prototype.NestedTypes = new NestedTypeContainer(nestedTypes); 
			prototype.AsContainerFunc = m_AsContainerFunc; 
			return Type::RegisterType<TClass>(m_Name, prototype, m_Attributes, m_DeclaringType);
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
		std::vector<Variant> m_Attributes;
		std::optional<std::function<ContainerView(Instance)>> m_AsContainerFunc;
	};
}

