#pragma once
#include "Property/PropertyData.h"
#include <map>

namespace RefLib
{
	class PropertyContainer
	{
	public:
		PropertyContainer(const std::vector<PropertyData>& datas);
		Ref<PropertyData> GetProp(const std::string& name);
		std::vector<PropertyData>& GetAll() { return m_Properties; }

	private:
		std::map<std::string, PropertyData> m_PropertyMap;
		std::vector<PropertyData> m_Properties;
	};
}

