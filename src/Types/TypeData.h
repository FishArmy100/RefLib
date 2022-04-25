#pragma once
#include <string_view>
#include <string>
#include "TypeId.h"
#include "TypeUtils.h"

namespace RefLib
{
	class TypeData
	{
	public:
		TypeData(const std::string& name, TypeId id) : m_Name(name), m_Id(id), m_IsRegistered(false) {}
		TypeData(const std::string& name, TypeId id, bool isRegistered) : m_Name(name), m_Id(id), m_IsRegistered(isRegistered) {}
		TypeData() : m_Id(NoneType), m_Name(""), m_IsRegistered(false) {}
		
		std::string_view GetName() const { return m_Name; }
		TypeId GetId() const { return m_Id; }
		bool GetIsRegistered() const { return m_IsRegistered; }

	private:
		TypeId m_Id;
		std::string m_Name;
		bool m_IsRegistered;
	};
}

