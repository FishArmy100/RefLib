#pragma once
#include "TypeBuilderBase.h"
#include "Enum/Enum.h"
#include "Enum/EnumData.h"
#include <map>

namespace RefLib
{
	template<typename TEnum>
	class EnumBuilder : public TypeBuilderBase
	{
	static_assert(std::is_enum_v<TEnum>, "Type must be an enum");

	public:
		EnumBuilder(const std::string& name, const std::map<std::string, TEnum>& values) :
			m_Name(name), m_Values(values)
		{
		}

		void AddValue(const std::string& name, TEnum value)
		{
			m_Values[name] = value; 
		}

		void AddValues(const std::map<std::string, TEnum>& values)
		{
			for (const auto&[name, value] : values)
			{
				m_Values[name] = value;
			}
		}

		Type Register()
		{
			return Type::RegisterEnum<TEnum>(m_Name, new EnumData<TEnum>(m_Name, m_Values), m_DeclaringType);
		}

	protected:
		void SetDeclaring(TypeId id) override
		{
			m_DeclaringType = id;
		}

	private:
		std::optional<TypeId> m_DeclaringType;
		std::string m_Name;
		std::map<std::string, TEnum> m_Values;
	};

}

