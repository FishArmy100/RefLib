#pragma once
#include "Types/Type.h"
#include <optional>

namespace RefLib
{
	class Reference;

	class Variant
	{
	public:
		Variant() : m_Type(Type::Invalid()), m_Data(nullptr), m_IsValid(false),
					m_CopyData(nullptr), m_DeleteData(nullptr) 
		{}

		template<typename T>
		Variant(const T data) : m_Type(Type::Get<T>()), m_Data(new T(data)), m_IsValid(true)
		{
			m_DeleteData = [](void* ptr) { delete(T*)ptr; };
			m_CopyData = [](void* ptr) { return (void*)new T(*(T*)ptr); };
		}

		Variant(const Variant& other) : 
			m_Type(other.m_Type), 
			m_DeleteData(other.m_DeleteData),
			m_CopyData(other.m_CopyData),
			m_IsValid(true)
		{
			if(other.m_IsValid)
				m_Data = m_CopyData(other.m_Data);
		}

		Variant& operator=(const Variant& other)
		{
			if (this == &other)
				return *this;

			if (other.m_IsValid)
				this->m_Data = other.m_CopyData(other.m_Data);
			else
				this->m_Data = nullptr;

			this->m_Type = other.m_Type;
			this->m_CopyData = other.m_CopyData;
			this->m_DeleteData = other.m_DeleteData;
			this->m_IsValid = other.m_IsValid;

			return *this;
		}

		template<>
		Variant(const Reference ref);

		Variant(Variant&&) = delete;

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

		~Variant()
		{
			if(m_Data != nullptr && !IsValid())
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

