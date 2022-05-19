#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include "Type.h"
#include "Method/MethodData.h"
#include "Property/PropertyData.h"
#include "Constructor/ConstructorData.h"

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

	using ConstructorContainer = std::vector<ConstructorData>;
}