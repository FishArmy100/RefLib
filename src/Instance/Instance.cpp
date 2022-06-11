#include "Instance.h"
#include "Variant/Variant.h"

namespace RefLib
{
	Instance::Instance(const Variant& v) : m_Type(v.GetType()), m_Data(v.GetRawData()) 
	{
		m_AsVarientFunc = [&]() { return v; };
	}

	Instance::Instance(const TempVariant tv) : m_Type(tv.DataType), m_Data(tv.Data)
	{
		m_AsVarientFunc = [=]() {return Variant(tv); };
	}
}