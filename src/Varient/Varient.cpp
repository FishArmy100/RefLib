#include "Varient.h"
#include "Reference/Reference.h"

namespace RefLib
{
	template<>
	Varient::Varient(const Reference ref) : m_Type(ref.GetType())
	{
		*this = ref.AsVarient();
	}
}