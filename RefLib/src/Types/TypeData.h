#pragma once
#include <string_view>
#include <string>
#include <string_view>
#include "TypeId.h"
#include "TypeUtils.h"
#include <vector>
#include <memory>
#include "TypeFlags.h"
#include <optional>
#include "Attributes/AttributeHolder.h"

namespace RefLib
{
	class PropertyData;
	class MethodData;
	class PropertyContainer;
	class MethodContainer;
	class ConstructorData;
	class Type;
	class EnumDataWrapper;
	enum class AccessLevel;
	class BaseTypeContainer;
	class NestedTypeContainer;

	struct TypeData
	{
		TypeData(const std::string& name, TypeId id) : 
			Name(name), Id(id), DereferenceFunc(nullptr),
			Properties(nullptr), Methods(nullptr), Constructors(nullptr),
			EnumValue(nullptr), BaseTypes(nullptr),
			NestedTypes(nullptr), PreRegisteredMethods(nullptr),
			Attributes(nullptr)
		{}
		TypeData(const TypeData& other) = default;
		~TypeData();

		std::string Name;
		TypeId Id;
		bool IsPointer = false;
		bool IsRegistered = false;

		Type(*DereferenceFunc)() = nullptr;

		std::optional<TypeId> DeclaringType;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;
		BaseTypeContainer* BaseTypes;
		NestedTypeContainer* NestedTypes;
		std::shared_ptr<AttributeHolder> Attributes;

		std::vector<MethodData>* PreRegisteredMethods;

		EnumDataWrapper* EnumValue;
	};
}

