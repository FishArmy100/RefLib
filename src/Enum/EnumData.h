#pragma once
#include "Types/Type.h"
#include "EnumDataWrapper.h"
#include <map>
#include <type_traits>
#include "Variant/Variant.h"

namespace RefLib
{
	template<typename TEnum> //TODO: make sure can only be an enum
	class EnumData : public EnumDataWrapper
	{
	public:
		EnumData(const std::string& name, const std::map<std::string, TEnum>& values) : 
			EnumDataWrapper(Type::Get<TEnum>(), Type::Get<std::underlying_type_t<TEnum>>(), name)
		{
			m_ValueMap = values;
			for (auto [name, value] : m_ValueMap)
			{
				m_NameMap[value] = name;
				m_Names.emplace_back(name);
				m_Values.emplace_back(value);
				m_VariantValues.push_back(Variant(value));
			}
		}

		Variant GetValueFromName(const std::string& name) override
		{
			return Variant(m_ValueMap[name]);
		}

		std::string_view GetNameFromValue(const Variant& value) override
		{
			if (value.GetType().GetId() == this->GetType().GetId())
				return m_NameMap[value.TryConvert<TEnum>().value()];

			return std::string_view();
		}

		Ref<const std::vector<Variant>> GetValues() override
		{
			return Ref<const std::vector<Variant>>(m_VariantValues);
		}

		Ref<const std::vector<std::string>> GetNames() override
		{
			return Ref<const std::vector<std::string>>(m_Names);
		}

	private:
		std::map<std::string, TEnum> m_ValueMap;
		std::map<TEnum, std::string> m_NameMap;
		std::vector<std::string> m_Names;
		std::vector<TEnum> m_Values;
		std::vector<Variant> m_VariantValues;
	};


}

