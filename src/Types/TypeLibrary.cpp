#include "TypeLibrary.h"

namespace RefLib
{
	void TypeLibrary::AddData(TypeData data)
	{
		m_Types[data.GetId()] = data;
	}

	const TypeData* TypeLibrary::GetData(TypeId id)
	{
		if (m_Types.find(id) != m_Types.end())
			return &m_Types[id];

		return nullptr;
	}
}