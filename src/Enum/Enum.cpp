#include "Enum.h"

namespace RefLib
{
	Type Enum::GetType()
	{
		return IsValid() ? m_Wrapper->GetType() : Type();
	}

	Type Enum::GetUnderlyingType()
	{
		return IsValid() ? m_Wrapper->GetUnderlyingType() : Type();
	}

	Type Enum::GetDeclaringType()
	{
		return IsValid() ? m_Wrapper->GetDeclaringType() : Type();
	}

	std::string_view Enum::GetName()
	{
		return IsValid() ? m_Wrapper->GetName() : std::string_view();
	}

	bool Enum::IsGlobalEnum()
	{
		return IsValid() ? m_Wrapper->GetDeclaringType() == Type::Invalid() : false;
	}

	Variant Enum::GetValueFromName(const std::string& name)
	{
		return IsValid() ? m_Wrapper->GetValueFromName(name) : Variant();
	}

	std::string_view Enum::GetNameFromValue(const Variant& value)
	{
		return IsValid() ? m_Wrapper->GetNameFromValue(value) : std::string_view();
	}

	Ref<const std::vector<Variant>> Enum::GetValues()
	{
		return IsValid() ? m_Wrapper->GetValues() : Ref<const std::vector<Variant>>();
	}

	Ref<const std::vector<std::string>> Enum::GetNames()
	{
		return IsValid() ? m_Wrapper->GetNames() : Ref<const std::vector<std::string>>();
	}

	std::pair<std::string_view, Variant> Enum::GetEnumValueFromIndex(size_t index)
	{
		if (m_Wrapper.IsNotNull())
		{
			auto names = GetNames();
			auto values = GetValues(); 

			if (names->size() < index)
				return std::make_pair(names->at(index), values->at(index));
		}

		return std::make_pair(std::string_view(), Variant());
	}
}