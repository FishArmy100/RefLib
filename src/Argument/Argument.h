#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"

namespace RefLib
{
	class Argument
	{
		template<typename T, typename Tp = Utils::decay_t<T>>
		using decay_reference_t = Utils::enable_if_t<!std::is_same<Argument, Tp>::value, T>;

	public:
		template<typename T, typename = typename decay_reference_t<T>>
		Argument(T&& arg)
		{
			m_Data = &arg;
			m_Type = Type::Get<T>();
		}

		Argument(const Argument& other) = default;
		Argument& operator=(const Argument& other)
		{
			this->m_Data = other.m_Data;
			this->m_Type = other.m_Type;
			return *this;
		}

		template<typename T>
		inline T& Get()
		{
			using TRaw = typename std::remove_reference_t<T>;
			return (*reinterpret_cast<TRaw*>(const_cast<void*>(m_Data)));
		}

		~Argument() = default;

		Type GetType() const { return m_Type; }
		template<typename T>
		bool IsConvertableTo() { return m_Type.IsAssignableFrom(Type::Get<T>()); }
		const void* GetData() const { return m_Data; }
		bool IsVarient() const { return m_Type.GetId() == Type::Get<Variant>().GetId(); }

	private:
		const void* m_Data;
		Type m_Type;
	};
}

