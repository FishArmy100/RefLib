#pragma once
#include <string_view>
#include <string>
#include <string_view>
#include "TypeId.h"
#include "TypeUtils.h"
#include <vector>
#include <memory>
#include "TypeFlags.h"
#include "MemberContianer.h"

namespace RefLib
{
	class PropertyData;
	class MethodData;
	class Type;

	struct TypeData
	{
		TypeData(const std::string& name, TypeId id) : Name(name), Id(id) {}
		TypeData(const TypeData& other) = default;
		~TypeData();

		std::string Name;
		TypeId Id;
		bool IsPointer = false;
		bool IsRegistered = false;

		Type(*DereferenceFunc)() = nullptr;

		MemberContainer<PropertyData*> Properties;
		MemberContainer<MethodData*> Methods;
	};
}

