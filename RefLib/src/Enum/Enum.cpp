#include "Enum.h"

namespace RefLib
{
	Type Enum::GetType()
	{
		return m_Wrapper->GetType();
	}

	Type Enum::GetUnderlyingType()
	{
		return m_Wrapper->GetUnderlyingType();
	}

	std::string_view Enum::GetName()
	{
		return m_Wrapper->GetName();
	}

	Variant Enum::GetValueFromName(const std::string& name)
	{
		return m_Wrapper->GetValueFromName(name);
	}

	std::string_view Enum::GetNameFromValue(const Variant& value)
	{
		return m_Wrapper->GetNameFromValue(value);
	}

	Ref<const std::vector<Variant>> Enum::GetValues()
	{
		return  m_Wrapper->GetValues();
	}

	Ref<const std::vector<std::string>> Enum::GetNames()
	{
		return m_Wrapper->GetNames();
	}

	std::optional<std::pair<std::string, Variant>> Enum::GetEnumValueFromIndex(size_t index)
	{
		auto names = GetNames();
		auto values = GetValues();

		if (names->size() > index)
			return std::make_pair(names->at(index), values->at(index));

		return {};
	}
}