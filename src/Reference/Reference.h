#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"
#include <functional>

namespace RefLib
{
	class Variant;

	class Reference
	{
	private:
		template<typename T, typename Tp = Utils::decay_t<T>>
		using decay_reference_t = Utils::enable_if_t<!std::is_same<Reference, Tp>::value &&
													 !std::is_same<Variant, Tp>::value, T>;

	public:
		Reference(const Reference& ref) = default;
		Reference(const Variant & v);

		template<typename T, typename Tp = decay_reference_t<T>>
		Reference(T& data) : m_Type(Type::Get<T>()), m_Data(&data)
		{
			m_AsVarientFunc = [&]() { return Variant(*(T*)m_Data); };
		}

		//Reference(Reference& ref) = default;
		~Reference() = default;

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

	private:
		Type m_Type;
		void* m_Data;
		std::function<Variant()> m_AsVarientFunc;
	};
}

