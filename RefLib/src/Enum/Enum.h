#pragma once
#include "EnumDataWrapper.h"
#include "Attributes/AttributeImpl.h"

namespace RefLib
{
	class Enum
	{
	public:
		Enum(Ref<EnumDataWrapper> enumWrapper) : m_Wrapper(enumWrapper) {};

		Type GetType();
		Type GetUnderlyingType();
		std::string_view GetName();

		Variant GetValueFromName(const std::string& name);
		std::string_view GetNameFromValue(const Variant& value);
		Ref<const std::vector<Variant>> GetValues();
		Ref<const std::vector<std::string>> GetNames();

		std::optional<std::pair<std::string, Variant>> GetEnumValueFromIndex(size_t index);

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*m_Wrapper->GetAttributeHolder());

	private:
		Ref<EnumDataWrapper> m_Wrapper;
	};
}

