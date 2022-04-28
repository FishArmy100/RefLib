#include "Property.h"
#include "PropertyData.h"

namespace RefLib
{
	Type Property::GetType()
	{
		return m_Data->GetType();
	}

	Type Property::GetDeclaringType()
	{
		return m_Data->GetDeclaringType();
	}
	std::string_view Property::GetName()
	{
		return m_Data->GetName();
	}
	Variant Property::Get(Reference ref)
	{
		return m_Data->Get(ref); 
	}
}