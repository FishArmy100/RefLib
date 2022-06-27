#include "DerivedTypeContainer.h"

namespace RefLib
{
	DerivedTypeContainer::DerivedTypeContainer(const std::vector<Type>& types) :
		m_DerivedTypes(types) 
	{
	}

	std::optional<Type> DerivedTypeContainer::GetDerivedType(const std::string& name) const
	{
		for (Type t : m_DerivedTypes)
		{
			if (t.GetName() == name)
				return t;
		}

		return {};
	}

	void DerivedTypeContainer::AddDerivedType(Type t)
	{
		m_DerivedTypes.push_back(t);
	}
}
