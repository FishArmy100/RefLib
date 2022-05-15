#include "Property.h"
#include "PropertyData.h"

namespace RefLib
{
	Type Property::GetType()
	{
		if (!IsValid())
			return Type::Invalid();

		return m_Data->GetType();
	}

	Type Property::GetDeclaringType()
	{
		if (!IsValid())
			return Type::Invalid();

		return m_Data->GetDeclaringType();
	}

	std::string_view Property::GetName()
	{
		if (!IsValid())
			return "";

		return m_Data->GetName();
	}

	Variant Property::Get(Instance ref)
	{
		if (!IsValid())
			return Variant();

		return m_Data->Get(ref); 
	}
}