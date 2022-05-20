#include "PropertyContainer.h"

namespace RefLib
{
	PropertyContainer::PropertyContainer(const std::vector<PropertyData>& datas)
	{
		for (auto& p : datas)
		{
			this->m_Properties.push_back(p);
			this->m_PropertyMap.insert(std::make_pair(p.Name, p));
		}
	}

	Ref<PropertyData> PropertyContainer::GetProp(const std::string& name)
	{
		auto foundIt = m_PropertyMap.find(name);
		if (foundIt != m_PropertyMap.end())
			return &foundIt->second;

		return nullptr;
	}
}