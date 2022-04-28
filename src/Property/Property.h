#pragma once
#include <string>
#include <string_view>
#include "PropertyData.h"

namespace RefLib
{
	class PropertyData;
	class Type;
	class Reference;
	class Variant;

	class Property
	{
	public:
		Property(PropertyData* data) : m_Data(data) {}
		Property() : m_Data(nullptr) {}

		Type GetType();
		Type GetDeclaringType();
		std::string_view GetName();

		Variant Get(Reference ref);
		
		template<typename T>
		bool Set(Reference ref, T&& data)
		{
			return m_Data->Set(ref, std::forward<T>(data));
		}

		bool IsValid() { return m_Data != nullptr; }

	private:
		PropertyData* m_Data;
	};
}

