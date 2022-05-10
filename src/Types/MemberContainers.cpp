#include "MemberContainers.h"
#include "Property/PropertyData.h"

namespace RefLib
{
	PropertyContainer::PropertyContainer(const std::vector<PropertyData>& datas)
	{
		for (auto& p : datas)
		{
			this->m_Properties.push_back(p);
			this->m_PropertyMap.insert(std::make_pair(p.GetName(), p));
		}
	}

	PropertyData* PropertyContainer::GetProp(const std::string& name)
	{
		PropertyData* data = &(m_PropertyMap[name]);
		if (data->IsValid())
			return data;

		return nullptr;
	}


	MethodContainer::MethodContainer(const std::vector<MethodData>& methods)
	{
		for (auto m : methods)
		{
			this->m_Methods.push_back(m);
			this->m_MethodMap[m.Name][m.SignatureType] = m;
		}
	}

	MethodData* MethodContainer::GetMethod(const std::string& name)
	{
		auto& overloadMap = m_MethodMap[name];
		auto it = overloadMap.begin();

		if (it == overloadMap.end())
			return nullptr;

		auto& pair = *it;
		MethodData* data = &(pair.second);
		if (!data->IsValid())
			return nullptr;

		return data;
	}

	MethodData* MethodContainer::GetOverloadedMethod(const std::string& name, Type signature)
	{
		MethodData* data = &(m_MethodMap[name][signature]);
		if (data->IsValid())
			return data;

		return nullptr;
	}
}