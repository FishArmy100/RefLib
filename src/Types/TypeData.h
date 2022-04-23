#pragma once
#include <string_view>
#include <string>

namespace RefLib
{
	class TypeData
	{
	public:
		TypeData(const std::string& name) : m_Name(name)
		{
			
		}

		std::string_view GetName() { return m_Name; }

	private:
		std::string m_Name;
	};
}

