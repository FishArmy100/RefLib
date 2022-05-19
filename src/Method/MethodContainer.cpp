#include "MethodContainer.h"

namespace RefLib
{
	MethodContainer::MethodContainer(const std::vector<MethodData>& methods)
	{
		for (auto& m : methods)
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

	Ref<MethodData> MethodContainer::GetMethod(const std::string& name)
	{
		auto overloadMap = m_MethodMap.find(name);
		if (overloadMap == m_MethodMap.end())
			return nullptr;

		auto it = overloadMap->second.begin();

		if (it == overloadMap->second.end())
			return nullptr;

		auto& pair = *it;
		return &(pair.second);
	}

	Ref<MethodData> MethodContainer::GetOverloadedMethod(const std::string& name, Type signature)
	{
		auto overloadMap = m_MethodMap.find(name);
		if (overloadMap == m_MethodMap.end())
			return nullptr;

		auto overloadedMethod = overloadMap->second.find(signature);
		if (overloadedMethod == overloadMap->second.end())
			return nullptr;

		return &overloadedMethod->second;
	}
}