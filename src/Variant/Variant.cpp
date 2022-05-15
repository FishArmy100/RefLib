#include "Variant.h"
#include "Instance/Instance.h"

namespace RefLib
{
	template<>
	Variant::Variant(const Instance ref) : m_Type(ref.GetType())
	{
		*this = ref.AsVarient();
	}

	template<>
	Variant::Variant(const TempVariant tv) :
		m_IsValid(true), m_Type(tv.DataType), m_Data(tv.Data),
		m_DeleteData(tv.DeleteData), m_CopyData(tv.CopyData), 
		m_DeleteDataPtr(tv.DeleteDataPtr)
	{

	}

	Variant Variant::GetVoidVarient()
	{
		return Variant(VarientVoidType{});
	}

	bool Variant::Delete()
	{
		bool didDelete = m_DeleteDataPtr(m_Data);
		if (didDelete)
		{
			m_Data = nullptr;
			*this = Variant();
			return true;
		}

		return false;
	}

	Instance Variant::GetDereferenced()
	{
		if (this->IsPointer())
		{
			auto opt = m_GetDereferencedHealper(m_Data);
			if (opt.has_value())
				return Instance(opt.value());
		}

		return Instance();
	}
}