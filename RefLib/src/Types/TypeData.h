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
#include <functional>
#include "TypeDataPrototype.h"

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
	class Instance;
	class ContainerView;

	struct TypeData
	{
		TypeData(const std::string& name, TypeId id, bool isPointer, std::function<Type()> dereferenceFunc);
		TypeData(const std::string& name, TypeId id, bool isPointer, TypeDataPrototype prototype, const std::vector<Variant>& attributes, std::function<Type()> dereferenceFunc, std::optional<TypeId> declaring);
		TypeData(const std::string& name, TypeId id, EnumDataWrapper* enumValue, std::function<Type()> dereferenceFunc, std::optional<TypeId> declaring);

		TypeData(const TypeData& other) = delete;
		~TypeData();

		std::string Name;
		TypeId Id;
		bool IsPointer = false;
		bool IsRegistered = false;

		std::function<Type()> DereferenceFunc;
		std::optional<std::function<ContainerView(Instance)>> AsContainerFunc;

		std::optional<TypeId> DeclaringType;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;
		BaseTypeContainer* BaseTypes;
		NestedTypeContainer* NestedTypes;
		std::unique_ptr<AttributeHolder> Attributes;

		std::vector<MethodData>* PreRegisteredMethods;

		std::optional<EnumDataWrapper*> EnumValue;
	};
}

