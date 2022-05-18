#pragma once

namespace RefLib
{
	class PropertyContainer;
	class MethodContainer;
	class ConstructorData;

	struct TypeDataPrototype
	{
		TypeDataPrototype() : Properties(nullptr), Methods(nullptr), Constructors(nullptr) {}
		TypeDataPrototype(const TypeDataPrototype& other) = default;
		~TypeDataPrototype() = default;

		PropertyContainer* Properties;
		MethodContainer* Methods;
		std::vector<ConstructorData>* Constructors;

		bool IsFullyBuilt() { return Properties != nullptr && Methods != nullptr && Constructors != nullptr; }
	};
}