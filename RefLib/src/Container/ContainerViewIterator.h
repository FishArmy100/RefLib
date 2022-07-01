#pragma once
#include "Types/Type.h"
#include <functional>
#include "Variant/Variant.h"
#include "Instance/Instance.h"

namespace RefLib
{
	class ContainerViewIterator
	{
	public:
		template<typename TIterator>
		ContainerViewIterator(TIterator it) :
			m_Iterator(it)
		{
			m_OpPlusPlus = [&]() -> ContainerViewIterator&
			{
				Instance i = m_Iterator;
				(*i.TryConvert<TIterator>())++;
				return *this;
			};

			m_Equles = [&](const ContainerViewIterator& other) -> bool
			{
				if (m_Iterator.GetType().GetId() != other.m_Iterator.GetType().GetId())
					return false;

				TIterator* aRef = (TIterator*)m_Iterator.GetRawData();
				TIterator* bRef = (TIterator*)other.m_Iterator.GetRawData();

				return *aRef == *bRef;
			};

			m_Dereference = [&]() -> Instance
			{
				Instance i = m_Iterator;
				TIterator* it = i.TryConvert<TIterator>();
				return **it;
			};
		}

		ContainerViewIterator operator++() { return m_OpPlusPlus(); }
		bool operator==(const ContainerViewIterator& other) { return m_Equles(other); }
		bool operator!=(const ContainerViewIterator& other) { return !(*this == other); }
		Instance operator*() { return m_Dereference(); }

	private:
		std::function<ContainerViewIterator& ()> m_OpPlusPlus;
		std::function<bool(const ContainerViewIterator&)> m_Equles;
		std::function<Instance()> m_Dereference;
		Variant m_Iterator;
	};
}