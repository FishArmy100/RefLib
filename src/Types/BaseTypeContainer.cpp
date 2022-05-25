#include "BaseTypeContainer.h"

namespace RefLib
{
	BaseTypeContainer::BaseTypeContainer(const std::vector<BaseType>& types) :
		m_Datas(types)
	{
	}

	std::optional<BaseType> BaseTypeContainer::GetBase(const std::string& name)
	{
		for (BaseType base : m_Datas)
		{
			if (base.Data.GetName() == name)
				return base;
		}

		return {};
	}
}