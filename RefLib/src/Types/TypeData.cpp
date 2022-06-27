#include "TypeData.h"
#include "Property/Property.h"
#include "Constructor/Constructor.h"
#include "Container/ContainerView.h"
#include "Property/PropertyContainer.h"
#include "Method/MethodContainer.h"
#include "BaseTypeContainer.h"
#include "NestedTypeContainer.h"
#include "Variant/Variant.h"
#include "Method/MethodData.h"
#include "Enum/EnumDataWrapper.h"
#include "DerivedTypeContainer.h"


namespace RefLib
{
	TypeData::TypeData(const std::string& name, TypeId id, bool isPointer, std::function<Type()> dereferenceFunc) :
		Name(name), Id(id), Properties(new PropertyContainer({})),
		DeclaringType(), DereferenceFunc(dereferenceFunc), AsContainerFunc(),
		Methods(new MethodContainer({})), Constructors(new std::vector<ConstructorData>()),
		BaseTypes(new BaseTypeContainer({})), DerivedTypes(new DerivedTypeContainer({})),
		NestedTypes(new NestedTypeContainer({})),
		Attributes(std::make_unique<AttributeHolder>(std::vector<Variant>())),
		PreRegisteredMethods(new std::vector<MethodData>()),
		EnumValue(std::optional<EnumDataWrapper*>()), 
		IsPointer(isPointer)
	{
		IsRegistered = false;
	}

	TypeData::TypeData(const std::string& name, TypeId id, bool isPointer, TypeDataPrototype prototype, const std::vector<Variant>& attributes, std::function<Type()> dereferenceFunc,  std::optional<TypeId> declaring) :
		Name(name), Id(id), DeclaringType(), 
		DereferenceFunc(dereferenceFunc), 
		AsContainerFunc(prototype.AsContainerFunc), 
		IsPointer(isPointer)
	{
		if (!prototype.IsFullyBuilt())
			throw std::exception("Prototype must be fully built");

		Properties = prototype.Properties;
		Methods = prototype.Methods;
		Constructors = prototype.Constructors;
		BaseTypes = prototype.BaseTypes;
		DerivedTypes = prototype.DerivedTypes;
		NestedTypes = prototype.NestedTypes;
		Attributes = std::make_unique<AttributeHolder>(attributes);

		IsRegistered = true;
	}

	TypeData::TypeData(const std::string& name, TypeId id, EnumDataWrapper* enumValue, std::function<Type()> dereferenceFunc, std::optional<TypeId> declaring) :
		TypeData(name, id, false, dereferenceFunc)
	{
		EnumValue = enumValue;
		DeclaringType = declaring;
		Attributes = std::make_unique<AttributeHolder>(enumValue->GetAttributeHolder()->GetAttributes());
		IsRegistered = true;
	}

	TypeData::~TypeData()
	{
		delete Properties;
		delete Methods;
		delete Constructors;
		
		if (EnumValue.has_value())
			delete EnumValue.value();

		delete BaseTypes;
		delete NestedTypes;
		delete DerivedTypes;
	}
}