#include "Variant.h"
#include "Reference/Reference.h"

namespace RefLib
{
	template<>
	Variant::Variant(const Reference ref) : m_Type(ref.GetType())
	{
		*this = ref.AsVarient();
	}

	Variant Variant::GetVoidVarient()
	{
		return Variant(VarientVoidType{});
	}
}