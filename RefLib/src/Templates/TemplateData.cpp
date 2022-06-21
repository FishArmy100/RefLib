#include "TemplateData.h"

namespace RefLib
{
	TemplateData::TemplateData(const std::optional<std::vector<TemplateParameter>>& params) : 
		m_IsTemplate(params.has_value()), m_Parameters(), m_ParamMap()
	{
		if (IsTemplate())
		{
			m_Parameters = params.value();
			for (size_t i = 0; i < m_Parameters.size(); i++)
				m_ParamMap[m_Parameters[i].Name] = i;
		}
	}

	std::optional<TemplateParameter> TemplateData::GetParameter(const std::string& name)
	{
		auto it = m_ParamMap.find(name);
		if (it != m_ParamMap.end())
			return m_Parameters[it->second];

		return {};
	}

	std::optional<TemplateParameter> TemplateData::GetParameter(size_t index)
	{
		if (m_Parameters.size() > index)
			return m_Parameters[index];

		return {};
	}

	const std::vector<TemplateParameter>& TemplateData::GetParameters()
	{
		return m_Parameters;
	}
}