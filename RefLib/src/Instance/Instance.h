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
		Instance(const Instance&) = default;
		Instance(Variant& v);
		Instance(TempVariant& tv);

		template<typename T, typename Tp = decay_reference_t<T>>
		Instance(T& data) : m_Type(Type::Get<T>()), m_Data(&data)
		{
			m_AsVarientFunc = [&]() { return Variant(*(T*)m_Data); };

			if constexpr (std::is_assignable_v<T, T>)
			{
				m_AssignValFunc = [&](Instance other)
				{
					T* selfValPtr = this->TryConvert<T>();
					T* otherValPtr = other.TryConvert<T>();

					if (selfValPtr == nullptr || otherValPtr == nullptr)
						return false;

					*selfValPtr = *otherValPtr;
					return true;
				};
			}
			else
			{
				m_AssignValFunc = {};
			}

			m_GetCopyFunc = [&]() { return new T(*(T*)m_Data); };
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
		bool AssignValue(Instance other) { return m_AssignValFunc.has_value() ? m_AssignValFunc.value()(other) : false; }

		Type GetType() const { return m_Type; }
		void* GetRawData() { return m_Data; }
		bool CanBeAssigned() { return m_AssignValFunc.has_value(); }

		bool IsValid() { return m_Data != nullptr; }

	private:

	private:
		Type m_Type;
		void* m_Data;
		std::function<Variant()> m_AsVarientFunc;
		std::optional<std::function<bool(Instance)>> m_AssignValFunc;
		std::function<void*()> m_GetCopyFunc;
	};
}

