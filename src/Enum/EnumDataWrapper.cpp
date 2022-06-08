#include "EnumDataWrapper.h"


namespace RefLib
{
	EnumDataWrapper::EnumDataWrapper(Type type, Type underlying, const std::string& name, const std::vector<Variant>& attributes) :
		m_Type(type), m_UnderlyingType(underlying), m_Name(name), m_Attributes(std::make_shared<AttributeHolder>(attributes))
	{

	}

	size_t EnumDataWrapper::GetNumberOfValues()
	{
		auto names = this->GetNames();
		if (!names.IsNull())
			return names->size();
		return 0;
	}
}
