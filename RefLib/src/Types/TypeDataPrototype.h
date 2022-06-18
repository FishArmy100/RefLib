#pragma once
#include <functional>
#include <optional>

namespace RefLib
{
	class PropertyContainer;
	class MethodContainer;
	class ConstructorData;
	struct TypeData;
	enum class AccessLevel;
	class BaseTypeContainer;
	class NestedTypeContainer;
	class ContainerView;

	struct TypeDataPrototype
	{
		TypeDataPrototype() : Properties(nullptr), Methods(nullptr), Constructors(nullptr), BaseTypes(nullptr), NestedTypes(nullptr), AsContainerFunc(nullptr) {}
		TypeDataPrototype(const TypeDataPrototype& other) = default;
		~TypeDataPrototype() = default;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;
		BaseTypeContainer* BaseTypes;
		NestedTypeContainer* NestedTypes;
		std::optional<std::function<ContainerView(Instance)>>* AsContainerFunc;

		bool IsFullyBuilt() { return Properties != nullptr && Methods != nullptr && Constructors != nullptr && BaseTypes != nullptr && NestedTypes != nullptr && AsContainerFunc != nullptr; }
	};
}