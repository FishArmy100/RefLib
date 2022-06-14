#include "AttributeHolder.h"
#include "Types/Type.h"
#include "Variant/Variant.h"

namespace RefLib
{
	AttributeHolder::AttributeHolder(const std::vector<Variant>& attributes) :
		m_Attributes(attributes), m_AttributeMap({})
	{
		for (int i = 0; i < m_Attributes.size(); i++)
			m_AttributeMap[m_Attributes[i].GetType().GetId()] = i;
	}

	Ref<const Variant> AttributeHolder::GetAttribute(Type t) const
	{
		auto it = m_AttributeMap.find(t.GetId()); 
		if (it != m_AttributeMap.end())
			return Ref<const Variant>(m_Attributes[it->second]);

		return {};
	}

	Ref<const Variant> AttributeHolder::GetAttribute(const std::string& name) const
	{
		std::optional<Type> type = Type::Get(name);
		if (type.has_value())
			return GetAttribute(type.value());

		return {};
	}

}