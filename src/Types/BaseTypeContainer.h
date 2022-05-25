#pragma once
#include "Types/Type.h"
#include <map>

namespace RefLib
{
	struct BaseType
	{
		BaseType(Type data, AccessLevel level) :
			Data(data), Level(level)
		{}

		Type Data;
		AccessLevel Level;
	};

	class BaseTypeContainer
	{
	public:
		BaseTypeContainer(const std::vector<BaseType>& types);

		std::vector<BaseType>& GetBaseTypes() { return m_Datas; }
		std::optional<BaseType> GetBase(const std::string& name);
	private:
		std::vector<BaseType> m_Datas;
	};
}

