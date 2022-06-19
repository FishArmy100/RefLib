#include "Variant.h"
#include "Instance/Instance.h"

namespace RefLib
{
	template<>
	Variant::Variant(const Instance& ref) : m_Type(ref.GetType())
	{
		*this = ref.AsVarient();
	}

	template<>
	Variant::Variant(const TempVariant& tv) :
		m_Type(tv.DataType), m_Data(tv.Data),
		m_DeleteData(tv.DeleteData), m_CopyData(tv.CopyData), 
		m_DeleteDataPtr(tv.DeleteDataPtr)
	{

	}

	Variant Variant::GetVoidVarient()
	{
		return Variant(VarientVoidType{});
	}

	Variant::Variant(Variant&& other) noexcept
	{
		m_Type = other.m_Type;
		m_Data = other.m_Data;
		m_DeleteData = other.m_DeleteData;
		m_CopyData = other.m_CopyData;
		m_DeleteDataPtr = other.m_DeleteDataPtr;
		m_GetDereferencedHealper = other.m_GetDereferencedHealper;

		other.m_Data = nullptr;
	}

	Variant& Variant::operator=(Variant&& other) noexcept
	{
		if (this == &other)
			return *this;

		if (m_Data != nullptr)
			m_DeleteData(m_Data);

		this->m_Type = other.m_Type;
		this->m_Data = other.m_Data;
		this->m_DeleteData = other.m_DeleteData;
		this->m_CopyData = other.m_CopyData;
		this->m_DeleteDataPtr = other.m_DeleteDataPtr;
		this->m_GetDereferencedHealper = other.m_GetDereferencedHealper;

		other.m_Data = nullptr;
		return *this;
	}

	std::optional<ContainerView> Variant::TryGetContainerView()
	{
		if (IsValid() && IsContainer())
			return m_Type.value().AsContainer(*this);
		return {};
	}

	bool Variant::Delete()
	{
		if (!IsValid())
			return false;

		bool didDelete = m_DeleteDataPtr(m_Data);
		if (didDelete)
		{
			m_Data = nullptr;
			*this = Variant();
			return true;
		}

		return false;
	}

	std::optional<Instance> Variant::GetDereferenced()
	{
		if (IsValid())
			return {};

		if (this->IsPointer())
		{
			auto opt = m_GetDereferencedHealper(m_Data);
			if (opt.has_value())
				return Instance(opt.value());
		}

		return {};
	}
}