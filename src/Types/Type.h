#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include "TypeLibrary.h"

namespace RefLib
{
	class Type
	{
	public:
		template<typename T>
		static Type Get()
		{
			return InternalGet<T>();
		}

		template<typename T>
		static bool RegisterType(const std::string&& name)
		{
			Type t = Get<T>();
			if (t.IsRegistered())
				return false;

			s_Library.AddData(TypeData(name, t.GetId(), true));
			return true;
		}

	public:
		Type(const TypeData* data) : m_Data(data) {}
		Type() : m_Data(nullptr) {}

		std::string_view GetName() const { return m_Data->GetName(); }
		TypeId GetId() const { return m_Data->GetId(); }
		bool IsRegistered() const { return m_Data->GetIsRegistered(); }

		static Type Invalid() { return Type(nullptr); }

		bool operator==(const Type& other) const
		{
			return GetId() == other.GetId();
		}

		bool operator!=(const Type& other) const
		{
			return GetId() != other.GetId();
		}

		bool IsValid() { return m_Data != nullptr; }

	private:
		template<typename T>
		static Type InternalGet()
		{
			static TypeId id = NoneType;
			if (id == NoneType)
			{
				id = s_CurrentId++;
				std::string name = Utils::GetTypeName<T>();
				//s_Library.AddData(TypeData(name, id));
			}

			return Type(s_Library.GetData(id));
		}

	private:
		static TypeId s_CurrentId;
		static TypeLibrary s_Library;

	private:
		const TypeData* m_Data;
	};
}

