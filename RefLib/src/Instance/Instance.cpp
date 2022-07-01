#include "Instance.h"
#include "Variant/Variant.h"

namespace RefLib
{
	Instance::Instance(Variant& v) : m_Type(v.GetType()), m_Data(v.GetRawData()) 
	{
		m_AsVarientFunc = [&]() { return v; };
		m_AssignValFunc = [&](Instance other) 
		{
			if (v.GetType().GetId() == other.GetType().GetId())
			{
				v.SetData(other.GetRawData());
				return true;
			}

			return false;
		};
	}

	Instance::Instance(TempVariant& tv) : m_Type(tv.DataType), m_Data(tv.Data)
	{
		m_AsVarientFunc = [=]() {return Variant(tv); };
		m_AssignValFunc = [=](Instance other)
		{
			if (tv.DataType.GetId() == other.GetType().GetId())
			{
				tv.SetData.value()(tv.Data, other.GetRawData());
				return true;
			}

			return false;
		};
	}
}