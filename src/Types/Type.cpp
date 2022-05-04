#include "Type.h"
#include "Property/Property.h"

namespace RefLib
{
	std::vector<TypeData*> Type::s_TypeDatas{};

	Property Type::GetProperty(const std::string& name)
	{
		return Property(this->m_Data->Properties[name]); 
	}

	std::vector<Property> Type::GetProperties()
	{
		return Utils::GetMemberVector<PropertyData*, Property>(this->m_Data->Properties, 
			[](auto& kv) 
			{ 
				return Property(kv.second); 
			});
	}

	bool Type::IsAssignableFrom(Type t)
	{
		if (this->IsConst())
			return false;

		return this->GetId() == t.GetId();
	}
}