#pragma once
#include <string>
#include <string_view>
#include "PropertyData.h"
#include "Types/Type.h"
#include "Utils/Ref.h"
#include "Attributes/AttributeImpl.h"

namespace RefLib
{
	struct PropertyData;
	class Instance;
	class Variant;

	class Property
	{
	public:
		Property(Ref<PropertyData> data);

		Type GetType() const;
		Type GetDeclaringType() const;
		std::string_view GetName() const;
		AccessLevel GetAccessLevel() const;

		Variant Get(Instance ref) const;
		bool Set(Instance ref, Argument arg) const;

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*m_Data->Attributes);

	private:
		Ref<PropertyData> m_Data;
	};
}

