#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"
#include <functional>
#include "Variant/TempVariant.h"

namespace RefLib
{
	class Variant;

	class Instance
	{
	private:
		template<typename TBase, typename Tp = Utils::decay_t<TBase>>
		using decay_reference_t = Utils::enable_if_t<!std::is_same<Instance, Tp>::value &&
													 !std::is_same<Variant, Tp>::value &&
													 !std::is_same<TempVariant, Tp>::value, TBase>;

	public:
		Instance(const Instance& other) = default;
		Instance(const Variant & v);
		Instance(const TempVariant tv);

		template<typename TBase, typename Tp = decay_reference_t<TBase>>
		Instance(TBase& data) : m_Type(Type::Get<TBase>()), m_Data(&data)
		{
			m_AsVarientFunc = [&]() { return Variant(*(TBase*)m_Data); };
		}

		~Instance() = default;

		template<typename TBase>
		TBase* TryConvert()
		{
			if (m_Type == Type::Get<TBase>())
				return (TBase*)m_Data;

			return nullptr;
		}

		Variant AsVarient() const { return m_AsVarientFunc(); }

		Type GetType() const { return m_Type; }
		void* GetRawData() { return m_Data; }

		bool IsValid() { return m_Data != nullptr; }

	private:
		Type m_Type;
		void* m_Data;
		std::function<Variant()> m_AsVarientFunc;
	};
}

