#include "Reference.h"
#include "Variant/Variant.h"

namespace RefLib
{
	Reference::Reference(const Variant& v) : m_Type(v.GetType()), m_Data(v.GetRawData()) 
	{
		m_AsVarientFunc = [&]() { return v; };
	}
}