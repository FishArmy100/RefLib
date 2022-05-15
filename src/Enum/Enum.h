#pragma once
#include "EnumDataWrapper.h"

namespace RefLib
{
	class Enum
	{
	public:
		Enum() : m_Wrapper(nullptr) {}
		Enum(Ref<EnumDataWrapper> enumWrapper) : m_Wrapper(enumWrapper) {};

		Type GetType();
		Type GetUnderlyingType();
		Type GetDeclaringType();
		std::string_view GetName();

		bool IsGlobalEnum();

		Variant GetValueFromName(const std::string& name);
		std::string_view GetNameFromValue(const Variant& value);
		Ref<const std::vector<Variant>> GetValues();
		Ref<const std::vector<std::string>> GetNames();

		std::pair<std::string_view, Variant> GetEnumValueFromIndex(size_t index);

		bool IsValid() { return !m_Wrapper.IsNull(); }

	private:
		Ref<EnumDataWrapper> m_Wrapper;
	};
}
