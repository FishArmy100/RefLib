#include "EnumDataWrapper.h"


namespace RefLib
{
	EnumDataWrapper::EnumDataWrapper(Type type, Type underlying, const std::string& name) :
		m_Type(type), m_UnderlyingType(underlying), m_Name(name)
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
