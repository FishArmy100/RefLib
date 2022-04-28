#include "TypeData.h"
#include "Property/Property.h"

namespace RefLib
{
	TypeData::~TypeData()
	{
		for (auto propPtr : m_Properties)
			delete propPtr;

		m_Properties.clear();
	}

	Property TypeData::GetProperty(std::string_view name)
	{
		PropertyData* prop = nullptr;
		for (auto p : m_Properties)
		{
			if (prop->GetName() == name)
			{
				prop = p;
				break;
			}
		}

		if (prop)
			return Property(prop);

		return Property();
	}
}