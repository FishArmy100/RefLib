#include "NestedTypeContainer.h"

namespace RefLib
{
	NestedTypeContainer::NestedTypeContainer(const std::vector<Type> types) :
		m_Types(types) 
	{
		for (Type type : types)
		{
			m_TypesMap.insert(std::make_pair(type.GetName(), type));
		}
	}

	std::optional<Type> NestedTypeContainer::GetType(const std::string& name)
	{
		auto it = m_TypesMap.find(name);
		if (it != m_TypesMap.end())
			return it->second;

		return {};
	}
}