#pragma once
#include <inttypes.h>
#include "TypeData.h"

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

			s_TypeDatas.emplace_back(TypeData(name, t.GetId(), t.GetFlags(), true));
			return true;
		}

	public:
		Type(const TypeData* data) : m_Data(data) {}
		Type() : m_Data(nullptr) {}

		std::string_view GetName() const { return m_Data->GetName(); }
		TypeId GetId() const { return m_Data->GetId(); }
		TypeFlags GetFlags() const { return m_Data->GetFlags(); }
		bool IsRegistered() const { return m_Data->GetIsRegistered(); }

		static Type Invalid() { return Type(nullptr); }

		bool IsFlag(TypeFlags flag) { return (bool)(m_Data->GetFlags() & flag); }
		bool IsConst() { return IsFlag(TypeFlags::Const); }
		bool IsVolotile() { return IsFlag(TypeFlags::Volatile); }
		bool IsLValue() { return IsFlag(TypeFlags::LValueReference); }
		bool IsRValue() { return IsFlag(TypeFlags::RValueReference); }

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
				id = s_TypeDatas.size();
				std::string name = Utils::GetTypeName<T>();
				TypeFlags flags = Utils::GetFlagsFromType<T>();
				s_TypeDatas.push_back(new TypeData(name, id, flags));
			}

			return Type(s_TypeDatas[id]);
		}

	private:
		static std::vector<TypeData*> s_TypeDatas;

	private:
		const TypeData* m_Data;
	};
}

