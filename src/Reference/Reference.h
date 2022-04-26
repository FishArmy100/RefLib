#pragma once
#include "Types/Type.h"
#include "Variant/Variant.h"

namespace RefLib
{
	class Variant;

	class Reference
	{
	public:
		template<typename T>
		Reference(T& data) : m_Type(Type::Get<T>()), m_Data(&data)
		{
			m_AsVarientFunc = [](void* d) { return Variant(*(T*)d); };
		}

		//Reference(Reference& ref) = default;
		~Reference() = default;

		Reference(Reference& ref) = default;

		template<>
		Reference(Variant& v);

		template<typename T>
		T* TryConvert()
		{
			if (m_Type == Type::Get<T>())
				return (T*)m_Data;

			return nullptr;
		}

		Variant AsVarient() const { return m_AsVarientFunc(m_Data); }

		Type GetType() const { return m_Type; }
		void* GetRawData() { return m_Data; }

	private:
		Type m_Type;
		void* m_Data;
		Variant(*m_AsVarientFunc)(void*);
	};
}

