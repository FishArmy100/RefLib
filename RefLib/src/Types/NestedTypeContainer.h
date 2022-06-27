#pragma once
#include "Type.h"
#include <map>

namespace RefLib
{
	class NestedTypeContainer
	{
	public:
		NestedTypeContainer(const std::vector<Type> types);
		const std::vector<Type>& GetTypes() { return m_Types; }
		std::optional<Type> GetType(const std::string& name);

	private:
		std::vector<Type> m_Types;
		std::map<std::string, Type> m_TypesMap;
	};
}

