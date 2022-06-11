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
				return Type(data->Id, TypeFlags::None);
		}

		return {};
	}

	std::optional<Type> Type::Get(TypeId id)
	{
		if (s_TypeDatas.size() > id && id != NoneType)
			return Type(id, TypeFlags::None);

		return {};
	}

	std::optional<Property> Type::GetProperty(const std::string& name) const
	{
		auto propData = this->GetData()->Properties->GetProp(name);
		if (propData.IsNotNull())
			return Property(propData);

		return {};
	}

	const std::vector<Property>& Type::GetProperties() const
	{
		std::vector<PropertyData>& datas = this->GetData()->Properties->GetAll();
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
		return this->GetData()->Methods->GetMethod(name);
	}

	std::optional<Method> Type::GetMethod(const std::string& name, const std::vector<Type>& paramTypes) const
	{
		auto container = this->GetData()->Methods;
		Ref<std::vector<size_t>> datas = container->GetMethods(name);
		for (auto& id : *datas.Get())
		{
			if (Utils::TypeListCanCallParamData(container->GetMethodData(id)->Parameters, paramTypes))
				return container->GetMethod(id); 
		}

		return {};
	}

	std::optional<Method> Type::GetTemplatedMethod(const std::string& name, const std::vector<Type>& templateArgs, const std::vector<Type>& paramTypes) const
	{
		auto container = this->GetData()->Methods;
		Ref<std::vector<size_t>> datas = container->GetTemplatedMethods(name);
		for (auto& id : *datas.Get())
		{
			auto data = container->GetMethodData(id);
			if (Utils::TypeListCanCallParamData(data->Parameters, paramTypes)&&
				data->TemplateParams == templateArgs)
			{
				return container->GetMethod(id);
			}
		}

		return {};
	}

	std::vector<Method> Type::GetMethods() const
	{
		auto containter = this->GetData()->Methods;
		std::vector<MethodData>& datas = containter->GetAll();
		std::vector<Method> methods;
		for (int i = 0; i < datas.size(); i++)
			methods.push_back(Method(i, containter));
		return methods;
	}

	Variant Type::InvokeMethod(const std::string& name, std::vector<Argument> args) const
	{
		if (!this->IsDefaultConstructable())
			return false;

		auto container = this->GetData()->Methods;
		Ref<std::vector<size_t>> datas = container->GetMethods(name); 
		for (auto& id : *datas.Get())
		{
			if (Utils::ArgListCanCallParamData(container->GetMethodData(id)->Parameters, args))
				return Method(id, container).Invoke(std::move(args));
		}

		return Variant();
	}

	Variant Type::InvokeMethod(Instance instance, const std::string& name, std::vector<Argument> args) const
	{
		if (this->GetId() != instance.GetType().GetId())
			return {};

		auto container = this->GetData()->Methods; 
		Ref<std::vector<size_t>> datas = container->GetMethods(name);
		for (auto& id : *datas.Get())
		{
			if (Utils::ArgListCanCallParamData(container->GetMethodData(id)->Parameters, args))
				return std::move(Method(id, container).Invoke(instance, std::move(args)));
		}

		return Variant();
	}

	Variant Type::InvokeTemplatedMethod(Instance instance, const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args) const
	{
		if (this->GetId() != instance.GetType().GetId())
			return {};

		auto container = this->GetData()->Methods;
		Ref<std::vector<size_t>> datas = container->GetTemplatedMethods(name);
		for (auto& id : *datas.Get())
		{
			auto methodData = container->GetMethodData(id);
			if (Utils::ArgListCanCallParamData(methodData->Parameters, args) &&
				methodData->TemplateParams == templateArgs)
			{
				return Method(id, container).Invoke(instance, std::move(args));
			}
		}

		return Variant();
	}

	Variant Type::InvokeTemplatedMethod(const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args) const
	{
		if (!this->IsDefaultConstructable())
			return false;

		auto container = this->GetData()->Methods;
		Ref<std::vector<size_t>> datas = container->GetTemplatedMethods(name);
		for (auto& id : *datas.Get())
		{
			auto methodData = container->GetMethodData(id);
			if (Utils::ArgListCanCallParamData(methodData->Parameters, args) &&
				methodData->TemplateParams == templateArgs)
			{
				return Method(id, container).Invoke(std::move(args));
			}
		}

		return Variant();
	}

	std::optional<Constructor> Type::GetConstructor(const std::vector<Type>& params) const
	{
		for (auto& constructorData : *(GetData()->Constructors))
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
		std::vector<ConstructorData>& datas = *(GetData()->Constructors);
		std::vector<Constructor> constructors;
		for (auto& data : datas)
			constructors.push_back(Constructor(&data));

		return constructors; 
	}

	Variant Type::Create(std::vector<Argument> args) const
	{
		for (auto& constructorData : *(GetData()->Constructors))
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
		for (auto& constructorData : *(GetData()->Constructors))
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
		for (auto& constructorData : *GetData()->Constructors)
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
			return Enum(Ref<EnumDataWrapper>(GetData()->EnumValue));
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
		return GetData()->BaseTypes->GetBaseTypes();
	}

	std::optional<Type> Type::GetDeclaringType()
	{
		if (IsNestedType())
			return Type(GetData()->DeclaringType.value(), TypeFlags::None);

		return {};
	}

	bool Type::IsNestedType()
	{
		return GetData()->DeclaringType.has_value();
	}

	std::optional<Type> Type::GetNestedType(const std::string& name) const
	{
		return GetData()->NestedTypes->GetType(name);
	}

	const std::vector<Type>& Type::GetNestedTypes() const
	{
		return GetData()->NestedTypes->GetTypes();
	}

	std::optional<BaseType> Type::GetBaseType(const std::string& name) const
	{
		return GetData()->BaseTypes->GetBase(name);
	}

	void Type::AddPreregisteredMethods(Ref<TypeData> data, MethodContainer* container)
	{
		if (data->PreRegisteredMethods == nullptr)
			return;

		for (auto& method : *data->PreRegisteredMethods)
			container->AddMethod(method);

		delete data->PreRegisteredMethods;
		data->PreRegisteredMethods = nullptr;
	}

	void Type::FillEnumTypeData(TypeData* typeData, EnumDataWrapper* enumData)
	{
		typeData->Properties = new PropertyContainer({});
		typeData->Methods = new MethodContainer({});
		typeData->Constructors = new std::vector<ConstructorData>();
		typeData->BaseTypes = new BaseTypeContainer({});
		typeData->NestedTypes = new NestedTypeContainer({});
		typeData->Attributes = enumData->GetAttributeHolder();
	}
}