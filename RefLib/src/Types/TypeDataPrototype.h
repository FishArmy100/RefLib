#pragma once

namespace RefLib
{
	class PropertyContainer;
	class MethodContainer;
	class ConstructorData;
	struct TypeData;
	enum class AccessLevel;
	class BaseTypeContainer;
	class NestedTypeContainer;

	struct TypeDataPrototype
	{
		TypeDataPrototype() : Properties(nullptr), Methods(nullptr), Constructors(nullptr), BaseTypes(nullptr), NestedTypes(nullptr) {}
		TypeDataPrototype(const TypeDataPrototype& other) = default;
		~TypeDataPrototype() = default;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;
		BaseTypeContainer* BaseTypes;
		NestedTypeContainer* NestedTypes;

		bool IsFullyBuilt() { return Properties != nullptr && Methods != nullptr && Constructors != nullptr && BaseTypes != nullptr && NestedTypes != nullptr; }
	};
}