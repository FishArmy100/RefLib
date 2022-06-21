#pragma once
#include "TemplateParameter.h"

namespace RefLib
{
	class TemplateData
	{
	public:
		TemplateData(const std::optional<std::vector<TemplateParameter>>& params);

		std::optional<TemplateParameter> GetParameter(const std::string& name);
		std::optional<TemplateParameter> GetParameter(size_t index);
		const std::vector<TemplateParameter>& GetParameters();
		bool IsTemplate() { return m_IsTemplate; }

	private:
		std::vector<TemplateParameter> m_Parameters;
		std::map<std::string, size_t> m_ParamMap;
		bool m_IsTemplate;
	};
}

