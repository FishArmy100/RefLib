#include "Reference.h"
#include "Varient/Varient.h"

namespace RefLib
{
	template<>
	Reference::Reference(Variant& v) : m_Type(v.GetType()), m_Data(v.GetRawData()) {}
}