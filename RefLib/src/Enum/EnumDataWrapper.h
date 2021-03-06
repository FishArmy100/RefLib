#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"
#include "Utils/Ref.h"
#include "Attributes/AttributeHolder.h"

namespace RefLib
{
	class EnumDataWrapper
	{
	public:
		EnumDataWrapper(Type type, Type underlying, const std::string& name, const std::vector<Variant>& attributes);

		Type GetType() { return m_Type; }
		Type GetUnderlyingType() { return m_UnderlyingType; }
		std::string_view GetName() { return m_Name; }

		virtual Variant GetValueFromName(const std::string& name) = 0;
		virtual std::string_view GetNameFromValue(const Variant& value) = 0;
		virtual Ref<const std::vector<Variant>> GetValues() = 0;
		virtual Ref<const std::vector<std::string>> GetNames() = 0;

		size_t GetNumberOfValues();
		std::shared_ptr<AttributeHolder> GetAttributeHolder() const { return m_Attributes; }

	protected:
		const Type m_Type;
		const Type m_UnderlyingType;
		const std::string m_Name;
		std::shared_ptr<AttributeHolder> m_Attributes;
	};
}

