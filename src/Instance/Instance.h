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
		template<typename T, typename Tp = Utils::decay_t<T>>
		using decay_reference_t = Utils::enable_if_t<!std::is_same<Instance, Tp>::value &&
													 !std::is_same<Variant, Tp>::value &&
													 !std::is_same<TempVariant, Tp>::value, T>;

	public:
		Instance() = default;

		Instance(const Instance& ref) = default;
		Instance(const Variant & v);
		Instance(const TempVariant tv);

		template<typename T, typename Tp = decay_reference_t<T>>
		Instance(T& data) : m_Type(Type::Get<T>()), m_Data(&data)
		{
			m_AsVarientFunc = [&]() { return Variant(*(T*)m_Data); };
		}

		~Instance() = default;

		template<typename T>
		T* TryConvert()
		{
			if (m_Type == Type::Get<T>())
				return (T*)m_Data;

			return nullptr;
		}

		Variant AsVarient() const { return m_AsVarientFunc(); }

		Type GetType() const { return m_Type; }
		void* GetRawData() { return m_Data; }

		bool IsValid() { return m_Data != nullptr; }

	private:
		Type m_Type = Type::Invalid();
		void* m_Data = nullptr;
		std::function<Variant()> m_AsVarientFunc = {};
	};
}

