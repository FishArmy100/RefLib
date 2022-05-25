#include "Type.h"
#include "Property/Property.h"
#include "Method/Method.h"
#include "Property/PropertyContainer.h"
#include "Constructor/Constructor.h"
#include "Argument/Argument.h"
#include "Variant/Variant.h"
#include "Instance/Instance.h"
#include "Enum/Enum.h"
#include "Method/MethodContainer.h"
#include "BaseTypeContainer.h"
#include "NestedTypeContainer.h"

namespace RefLib
{
	std::vector<TypeData*> Type::s_TypeDatas{};

	std::optional<Type> Type::Get(const std::string& name)
	{
		for (auto& data : s_TypeDatas)
		{
			if (data->Name == name)
				return Type(data, TypeFlags::None);
		}

		return {};
	}

	std::optional<Type> Type::Get(TypeId id)
	{
		if (s_TypeDatas.size() > id && id != NoneType)
			return Type(s_TypeDatas[id], TypeFlags::None);

		return {};
	}

	std::optional<Property> Type::GetProperty(const std::string& name) const
	{
		auto propData = this->m_Data->Properties->GetProp(name);
		if (propData.IsNotNull())
			return Property(propData);

		return {};
	}

	const std::vector<Property>& Type::GetProperties() const
	{
		std::vector<PropertyData>& datas = this->m_Data->Properties->GetAll();
		std::vector<Property> props;;
		for (int i = 0; i < datas.size(); i++)
			props.push_back(Property(&(datas[i])));
		return props;
	}

	Variant Type::GetPropertyValue(Instance instance, const std::string& propName) const
	{
		std::optional<Property> prop = GetProperty(propName);
		if (prop.has_value())
			return prop->Get(instance);

		return Variant();
	}

	bool Type::SetPropertyValue(Instance instance, const std::string& propName, Argument arg) const
	{
		std::optional<Property> prop = GetProperty(propName);
		if (prop.has_value())
			return prop->Set(instance, arg);

		return false;
	}

	std::optional<Method> Type::GetMethod(const std::string& name) const
	{
		Ref<MethodData> methodData = this->m_Data->Methods->GetMethod(name);
		if (methodData.IsNotNull())
			return Method(methodData);

		return {};
	}

	std::optional<Method> Type::GetMethod(const std::string& name, const std::vector<Type>& paramTypes) const
	{
		std::vector<MethodData>& datas = this->m_Data->Methods->GetAll();
		for (auto& methodData : datas) 
		{
			if (Utils::TypeListCanCallParamData(methodData.Parameters, paramTypes))
				return Method(&methodData);
		}

		return {};
	}

	std::vector<Method> Type::GetMethods() const
	{
		std::vector<MethodData>& datas = this->m_Data->Methods->GetAll();
		std::vector<Method> methods;
		for (int i = 0; i < datas.size(); i++)
			methods.push_back(Method(&(datas[i])));
		return methods;
	}

	Variant Type::InvokeMethod(const std::string& name, std::vector<Argument> args) const
	{
		if (!this->IsDefaultConstructable())
			return false;

		std::vector<MethodData>& datas = this->m_Data->Methods->GetAll();
		for (auto& methodData : datas)
		{
			if (Utils::ArgListCanCallParamData(methodData.Parameters, args))
				return Method(&methodData).Invoke(std::move(args));
		}

		return Variant();
	}

	Variant Type::InvokeMethod(Instance instance, const std::string& name, std::vector<Argument> args) const
	{
		std::vector<MethodData>& datas = this->m_Data->Methods->GetAll();
		for (auto& methodData : datas)
		{
			if (Utils::ArgListCanCallParamData(methodData.Parameters, args))
				return Method(&methodData).Invoke(instance, std::move(args));
		}

		return Variant();
	}

	std::optional<Constructor> Type::GetConstructor(const std::vector<Type>& params) const
	{
		for (auto& constructorData : *(m_Data->Constructors))
		{
			if (Utils::TypeListCanCallParamData(constructorData.Parameters, params))
			{
				return Constructor(&constructorData);
			}
		}

		return {};
	}

	std::vector<Constructor> Type::GetConstructors() const
	{
		std::vector<ConstructorData>& datas = *(m_Data->Constructors);
		std::vector<Constructor> constructors;
		for (auto& data : datas)
			constructors.push_back(Constructor(&data));

		return constructors; 
	}

	Variant Type::Create(std::vector<Argument> args) const
	{
		for (auto& constructorData : *(m_Data->Constructors))
		{
			if (Utils::ArgListCanCallParamData(constructorData.Parameters, args))
			{
				return Constructor(&constructorData).Construct(std::move(args));
			}
		}

		return Variant();
	}

	Variant Type::CreatePtr(std::vector<Argument> args) const
	{
		for (auto& constructorData : *(m_Data->Constructors))
		{
			if (Utils::ArgListCanCallParamData(constructorData.Parameters, args))
			{
				return Constructor(&constructorData).ConstructPtr(std::move(args));
			}
		}

		return Variant();
	}

	bool Type::IsDefaultConstructable() const
	{
		for (auto& constructorData : *m_Data->Constructors)
		{
			if (constructorData.Parameters.size() == 0)
				return true;
		}

		return false;
	}

	std::optional<Enum> Type::AsEnum() const
	{
		if (IsEnum())
		{
			return Enum(Ref<EnumDataWrapper>(m_Data->EnumValue));
		}

		return {};
	}

	bool Type::IsAssignableFrom(Type t) const
	{
		if (this->IsConst())
			return false;

		return this->GetId() == t.GetId();
	}

	bool Type::IsConvertableTo(Type type) const
	{
		if (this->GetId() != type.GetId())
			return false;

		if (this->IsConst() && (type.IsRef() || type.IsRValueRef()) && !type.IsConst())
			return false;

		return true;
	}

	bool Type::IsBaseOf(Type t) const
	{
		for (const auto& base : t.GetBaseTypes())
		{
			if (base.Data.GetId() == this->GetId())
				return true;

			if (this->IsBaseOf(base.Data))
				return true;
		}

		return false;
	}

	bool Type::IsDerivedFrom(Type type) const
	{
		for (const auto& base : this->GetBaseTypes())
		{
			if (base.Data.GetId() == type.GetId())
				return true;

			if (base.Data.IsDerivedFrom(type))
				return true;
		}

		return false;
	}

	const std::vector<BaseType>& Type::GetBaseTypes() const
	{
		return m_Data->BaseTypes->GetBaseTypes();
	}

	std::optional<Type> Type::GetDeclaringType()
	{
		if (IsNestedType())
			return Type(s_TypeDatas[m_Data->DeclaringType.value()], TypeFlags::None); 

		return {};
	}

	bool Type::IsNestedType()
	{
		return m_Data->DeclaringType.has_value();
	}

	std::optional<Type> Type::GetNestedType(const std::string& name) const
	{
		return m_Data->NestedTypes->GetType(name);
	}

	const std::vector<Type>& Type::GetNestedTypes() const
	{
		return m_Data->NestedTypes->GetTypes();
	}

	std::optional<BaseType> Type::GetBaseType(const std::string& name) const
	{
		return m_Data->BaseTypes->GetBase(name);
	}
}