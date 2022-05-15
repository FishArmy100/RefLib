#pragma once
#include <string>
#include <string_view>
#include "PropertyData.h"
#include "Types/Type.h"

namespace RefLib
{
	class PropertyData;
	class Instance;
	class Variant;

	class Property
	{
	public:
		Property(PropertyData* data) : m_Data(data) {}
		Property() : m_Data(nullptr) {}

		Type GetType();
		Type GetDeclaringType();
		std::string_view GetName();

		Variant Get(Instance ref);
		
		bool Set(Instance ref, Argument arg)
		{
			if (!IsValid())
				return false;

			return m_Data->Set(ref, arg);
		}

		bool IsValid() { return m_Data != nullptr; }

	private:
		PropertyData* m_Data;
	};
}

