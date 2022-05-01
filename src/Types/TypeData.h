#pragma once
#include <string_view>
#include <string>
#include <string_view>
#include "TypeId.h"
#include "TypeUtils.h"
#include <vector>
#include <memory>
#include "TypeFlags.h"

namespace RefLib
{
	class PropertyData;
	class Property;

	class TypeData
	{
	public:
		TypeData(const std::string& name, TypeId id, TypeFlags flags = TypeFlags::None, bool isRegistered = false) : 
			m_Name(name), m_Id(id), m_Flags(flags), m_IsRegistered(isRegistered) {}

		TypeData() : m_Id(NoneType), m_Name(""), m_Flags(TypeFlags::None), m_IsRegistered(false) {}

		virtual ~TypeData();
		
		std::string_view GetName() const { return m_Name; }
		TypeId GetId() const { return m_Id; }
		TypeFlags GetFlags() const { return m_Flags; }
		bool GetIsRegistered() const { return m_IsRegistered; }

		Property GetProperty(std::string_view name);

		bool IsValid() { return m_Id != NoneType; }

	private:
		std::string m_Name;
		TypeId m_Id;
		TypeFlags m_Flags;
		bool m_IsRegistered;

		std::vector<PropertyData*> m_Properties{};
	};
}

