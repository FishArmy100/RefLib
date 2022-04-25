#include "Reference.h"
#include "Varient/Varient.h"

namespace RefLib
{
	template<>
	Reference::Reference(Varient& v) : m_Type(v.GetType()), m_Data(v.GetRawData()) {}
}