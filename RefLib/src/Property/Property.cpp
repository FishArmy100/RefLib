#include "Property.h"
#include "PropertyData.h"

namespace RefLib
{
	Property::Property(Ref<PropertyData> data) : m_Data(data)
	{
		if (data.IsNull())
			throw std::exception("Cannot create a property with a nullptr");
	}

	AccessLevel Property::GetAccessLevel() const
	{
		return m_Data->Level;
	}

	Type Property::GetType() const
	{
		return m_Data->PropType;
	}

	Type Property::GetDeclaringType() const
	{
		return m_Data->DeclaringType;
	}

	std::string_view Property::GetName() const
	{
		return m_Data->Name;
	}

	Variant Property::Get(Instance ref) const
	{
		return m_Data->GetFunc(ref); 
	}

	bool Property::Set(Instance ref, Argument arg) const
	{
		return m_Data->SetFunc(ref, arg);
	}
}