#include "MemberContainers.h"
#include "Property/PropertyData.h"

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


	MethodContainer::MethodContainer(const std::vector<MethodData>& methods)
	{
		for (auto m : methods)
		{
			this->m_Methods.push_back(m);

			if (m_MethodMap.find(m.Name) == m_MethodMap.end())
			{
				m_MethodMap.insert({ m.Name, std::unordered_map<Type, MethodData>() });
			}

			auto& overloadMap = m_MethodMap.at(m.Name);

			if (overloadMap.find(m.SignatureType) == overloadMap.end())
			{
				overloadMap.insert({ m.SignatureType, m });
			}
		}
	}

	MethodData* MethodContainer::GetMethod(const std::string& name)
	{
		auto& overloadMap = m_MethodMap.at(name);
		auto it = overloadMap.begin();

		if (it == overloadMap.end())
			return nullptr;

		auto& pair = *it;
		return &(pair.second);
	}

	MethodData* MethodContainer::GetOverloadedMethod(const std::string& name, Type signature)
	{
		return &(m_MethodMap.at(name).at(signature));
	}
}