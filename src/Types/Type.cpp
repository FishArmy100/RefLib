#include "Type.h"
#include "Property/Property.h"
#include "Method/Method.h"
#include "MemberContainers.h"
#include "Constructor/Constructor.h"
#include "Argument/Argument.h"
#include "Variant/Variant.h"
#include "Enum/Enum.h"

namespace RefLib
{
	std::vector<TypeData*> Type::s_TypeDatas{};

	Property Type::GetProperty(const std::string& name)
	{
		return Property(this->m_Data->Properties->GetProp(name));
	}

	const std::vector<Property>& Type::GetProperties()
	{
		std::vector<PropertyData>& datas = this->m_Data->Properties->GetAll();
		std::vector<Property> props = std::vector<Property>(datas.size());
		for (int i = 0; i < props.size(); i++)
			props[i] = Property(&(datas[i]));
		return props;
	}

	Method Type::GetMethod(const std::string& name)
	{
		return Method(this->m_Data->Methods->GetMethod(name));
	}

	Method Type::GetOverloadedMethod(const std::string& name, Type signature)
	{
		return Method(this->m_Data->Methods->GetOverloadedMethod(name, signature));
	}

	std::vector<Method> Type::GetMethods()
	{
		std::vector<MethodData>& datas = this->m_Data->Methods->GetAll();
		std::vector<Method> methods = std::vector<Method>(datas.size());
		for (int i = 0; i < methods.size(); i++)
			methods[i] = Method(&(datas[i]));
		return methods;
	}

	Constructor Type::GetConstructor(const std::vector<Type>& params)
	{
		for (auto& constructorData : *(m_Data->Constructors))
		{
			if (Utils::TypeListCanCallParamData(constructorData.Parameters, params))
			{
				return Constructor(&constructorData);
			}
		}

		return Constructor();
	}

	std::vector<Constructor> Type::GetConstructors()
	{
		std::vector<ConstructorData>& datas = *(m_Data->Constructors);
		std::vector<Constructor> constructors;
		for (auto& data : datas)
			constructors.push_back(Constructor(&data));

		return constructors; 
	}

	Variant Type::Create(std::vector<Argument> args)
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

	Variant Type::CreatePtr(std::vector<Argument> args)
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

	Enum Type::AsEnum() const
	{
		if (IsValid() && IsEnum())
		{
			return Enum(Ref<EnumDataWrapper>(m_Data->EnumValue));
		}

		return Enum();
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
}