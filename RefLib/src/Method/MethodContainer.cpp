#include "MethodContainer.h"
#include "Method/Method.h"

namespace RefLib
{
	MethodContainer::MethodContainer(const std::vector<MethodData>& methods)
	{
		for (auto& m : methods)
		{
			AddMethod(m);
		}
	}

	std::optional<Method> MethodContainer::GetMethod(const std::string& name)
	{
		auto methods = GetMethods(name);
		if (methods.IsNotNull() && methods->size() > 0)
			return Method(0, Ref(this));

		return {};
	}

	std::optional<Method> MethodContainer::GetMethod(size_t id)
	{
		if (id < m_Methods.size())
			return Method(id, this);

		return {};
	}

	Ref<MethodData> MethodContainer::GetMethodData(size_t id)
	{
		if (id < m_Methods.size())
			return m_Methods.at(id);
	}

	Ref<std::vector<size_t>> MethodContainer::GetMethods(const std::string& name)
	{
		auto methods = m_MethodsMap.find(name);
		if (methods != m_MethodsMap.end())
			return methods->second;

		return {};
	}


	Ref<std::vector<size_t>> MethodContainer::GetTemplatedMethods(const std::string& name)
	{
		auto methods = m_TemplatedMethodsMap.find(name);
		if (methods != m_TemplatedMethodsMap.end())
			return methods->second;

		return {};
	}

	void MethodContainer::AddMethod(const MethodData& data)
	{
		this->m_Methods.push_back(data); 

		auto method = Ref(&m_Methods.at(m_Methods.size() - 1));

		if (data.IsTemplated())
			this->m_TemplatedMethodsMap[data.Name].push_back(m_Methods.size() - 1);
		else
			this->m_MethodsMap[data.Name].push_back(m_Methods.size() - 1);
	}
}