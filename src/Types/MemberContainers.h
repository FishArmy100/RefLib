#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include "Type.h"
#include "Method/MethodData.h"
#include "Property/PropertyData.h"

namespace RefLib
{
	class PropertyContainer
	{
	public:
		PropertyContainer(const std::vector<PropertyData>& datas);
		PropertyData* GetProp(const std::string& name);
		std::vector<PropertyData>& GetAll() { return m_Properties; }

	private:
		std::map<std::string, PropertyData> m_PropertyMap;
		std::vector<PropertyData> m_Properties;
	};

	class MethodContainer
	{
	public:
		MethodContainer(const std::vector<MethodData>& methods);

		MethodData* GetMethod(const std::string& name);
		MethodData* GetOverloadedMethod(const std::string& name, Type signature);
		std::vector<MethodData>& GetAll() { return m_Methods; }

	private:
		std::map<std::string, std::unordered_map<Type, MethodData>> m_MethodMap;
		std::vector<MethodData> m_Methods;
	};
}