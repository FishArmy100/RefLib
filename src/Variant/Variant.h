#pragma once
#include "Types/Type.h"
#include <optional>
#include "VarientVoidType.h"
#include <type_traits>
#include "TempVariant.h"
#include <functional>

namespace RefLib
{
	class Instance;

	class Variant
	{
	public:
		static Variant GetVoidVarient();

	public:
		Variant() : m_Type(Type::Invalid()), m_Data(nullptr), m_IsValid(false),
					m_CopyData(nullptr), m_DeleteData(nullptr), 
					m_DeleteDataPtr(nullptr), m_GetDereferencedHealper(nullptr)
		{}

		template<typename T>
		Variant(const T data) : m_Type(Type::Get<T>()), m_Data(new T(data)), m_IsValid(true)
		{
			m_DeleteData = [](void* ptr) { delete(T*)ptr; };
			m_CopyData = [](void* ptr) { return (void*)new T(*(T*)ptr); };
			m_DeleteDataPtr = [](void* ptr)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					delete* (T*)ptr;
					return true;
				}

				return false;
			};

			m_GetDereferencedHealper = [=](void*)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					void* derefData = *(T*)m_Data;
					return std::optional<TempVariant>(TempVariant::Create<std::remove_pointer_t<T>>(derefData));
				}

				return std::optional<TempVariant>{}; 
			};
		}

		Variant(const Variant& other) : 
			m_Type(other.m_Type), 
			m_DeleteData(other.m_DeleteData),
			m_CopyData(other.m_CopyData),
			m_IsValid(other.m_IsValid),
			m_DeleteDataPtr(other.m_DeleteDataPtr),
			m_GetDereferencedHealper(other.m_GetDereferencedHealper)
		{
			if (other.m_IsValid)
				m_Data = m_CopyData(other.m_Data);
			else
				m_Data = nullptr;
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
			this->m_DeleteDataPtr = other.m_DeleteDataPtr;
			this->m_GetDereferencedHealper = other.m_GetDereferencedHealper;

			return *this;
		}

		template<>
		Variant(const Instance ref);

		template<>
		Variant(const TempVariant tv);

		Variant(Variant&&) = delete;

		template<typename T>
		std::optional<T> TryConvert() const
		{
			if (!m_IsValid)
				return {};

			if (m_Type == Type::Get<T>())
				return *(T*)m_Data;

			return {};
		}

		Type GetType() const { return m_Type; }
		void* GetRawData() const { return m_Data; }
		bool IsValid() const { return m_IsValid; }
		bool IsVoid() const { return m_Type == Type::Get<VarientVoidType>(); }

		bool IsPointer() const { return m_Type.IsPointer(); }

		bool Delete();

		Instance GetDereferenced();

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
		bool(*m_DeleteDataPtr)(void*);
		std::function<std::optional<TempVariant>(void*)> m_GetDereferencedHealper;
	};
}
