#pragma once
#include "Types/Type.h"
#include <optional>

namespace RefLib
{
	class Reference;

	class Varient
	{
	public:
		Varient() : m_Type(Type::Invalid()), m_Data(nullptr), m_IsValid(false),
					m_CopyData(nullptr), m_DeleteData(nullptr) 
		{}

		template<typename T>
		Varient(const T data) : m_Type(Type::Get<T>()), m_Data(new T(data)), m_IsValid(true)
		{
			m_DeleteData = [](void* ptr) { delete(T*)ptr; };
			m_CopyData = [](void* ptr) { return (void*)new T(*(T*)ptr); };
		}

		Varient(const Varient& other) : 
			m_Type(other.m_Type), 
			m_DeleteData(other.m_DeleteData),
			m_CopyData(other.m_CopyData),
			m_IsValid(true)
		{
			m_Data = m_CopyData(other.m_Data);
		}

		Varient& operator=(const Varient& other)
		{
			if (this == &other)
				return *this;

			this->m_Data = other.m_CopyData(other.m_Data);
			this->m_Type = other.m_Type;
			this->m_CopyData = other.m_CopyData;
			this->m_DeleteData = other.m_DeleteData;
			this->m_IsValid = other.m_IsValid;

			return *this;
		}

		template<>
		Varient(const Reference ref);

		Varient(Varient&&) = delete;

		template<typename T>
		std::optional<T> TryConvert() const
		{
			if (m_Type == Type::Get<T>())
				return *(T*)m_Data;

			return {};
		}

		Type GetType() const { return m_Type; }
		void* GetRawData() const { return m_Data; }
		bool IsValid() { return m_IsValid; }

		~Varient()
		{
			m_DeleteData(m_Data);
		}

	private:
		bool m_IsValid;
		Type m_Type;
		void* m_Data;

		void(*m_DeleteData)(void*);
		void*(*m_CopyData)(void*);
	};
}

