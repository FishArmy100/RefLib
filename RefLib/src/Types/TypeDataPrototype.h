#pragma once
#include <functional>
#include <optional>
#include <unordered_set>

namespace RefLib
{
	class PropertyContainer;
	class MethodContainer;
	struct ConstructorData;
	struct TypeData;
	enum class AccessLevel;
	class BaseTypeContainer;
	class NestedTypeContainer;
	class ContainerView;
	class Instance;
	class DerivedTypeContainer;

	struct TypeDataPrototype
	{
		TypeDataPrototype() : Properties(nullptr), Methods(nullptr), Constructors(nullptr), BaseTypes(nullptr), NestedTypes(nullptr), AsContainerFunc(), DerivedTypes(nullptr) {}
		TypeDataPrototype(const TypeDataPrototype& other) = default;
		~TypeDataPrototype() = default;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;
		BaseTypeContainer* BaseTypes;
		DerivedTypeContainer* DerivedTypes;
		NestedTypeContainer* NestedTypes;
		std::optional<std::function<ContainerView(Instance)>> AsContainerFunc;

		bool IsFullyBuilt() { return Properties != nullptr && Methods != nullptr && Constructors != nullptr && BaseTypes != nullptr && NestedTypes != nullptr && DerivedTypes != nullptr; }
	};
}