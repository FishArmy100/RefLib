#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include <iostream>

namespace RefLib
{
	class Property;
	class Method;

	class Type
	{
	public:
		template<typename T>
		static Type Get()
		{
			using Undecorated = typename std::remove_cv<std::remove_reference<T>::type>::type;

			TypeFlags flags = Utils::GetFlagsFromType<T>();
			using NonReference = typename std::remove_reference_t<T>;
			flags = flags | Utils::GetFlagsFromType<NonReference>();

			return InternalGet<Undecorated>(flags);
		}

		template<typename T>
		static bool RegisterType(const std::string& name, MemberContainer<PropertyData*>& properties, MemberContainer<MethodData*>& methods)
		{
			Type t = Get<T>();
			if (t.IsRegistered())
				return false;

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true);
			data->Properties = properties;
			data->Methods = methods;
			s_TypeDatas[data->Id] = data;
			return true;
		}

	public:
		Type(TypeData* data, TypeFlags flags) : m_Data(data), m_Flags(flags) {}
		Type() : m_Data(nullptr), m_Flags(TypeFlags::None) {}
		Type(const Type& other) = default;
		Type& operator=(const Type& other) { this->m_Data = other.m_Data; this->m_Flags = other.m_Flags; return *this; }
		~Type() = default;

		std::string_view GetName() const { return m_Data->Name; }
		TypeId GetId() const { return m_Data->Id; }
		TypeFlags GetFlags() const { return m_Flags; }
		bool IsRegistered() const { return m_Data->IsRegistered; }

		static Type Invalid() { return Type(nullptr, TypeFlags::None); }

		bool IsFlag(TypeFlags flag) { return (bool)(m_Flags & flag); }
		bool IsConst() { return IsFlag(TypeFlags::Const); }
		bool IsVolotile() { return IsFlag(TypeFlags::Volatile); }
		bool IsLValueRef() { return IsFlag(TypeFlags::LValueReference); }
		bool IsRValueRef() { return IsFlag(TypeFlags::RValueReference); }

		Property GetProperty(const std::string& name);
		std::vector<Property> GetProperties();

		Method GetMethod(const std::string& name);
		std::vector<Method> GetMethods();

		bool IsPointer() { return m_Data->IsPointer; }

		bool IsAssignableFrom(Type t);

		Type Dereferenced() { return m_Data->DereferenceFunc(); }

		bool operator==(const Type& other) const
		{
			return GetId() == other.GetId() && m_Flags == other.GetFlags();
		}

		bool operator!=(const Type& other) const
		{
			return !(*this == other);
		}

		bool IsValid() { return m_Data != nullptr; }

	private:
		template<typename T>
		static Type InternalGet(TypeFlags flags)
		{
			static TypeId id = NoneType;
			if (id == NoneType)
			{
				id = s_TypeDatas.size();
				std::string name = Utils::GetTypeName<T>();
				s_TypeDatas.push_back(CreateTypeData<T>(name, id, false));
			}

			return Type(s_TypeDatas[id], flags);
		}

		template<typename T>
		static TypeData* CreateTypeData(const std::string& name, TypeId id, bool isRegistered)
		{
			TypeData* data = new TypeData(name, id);
			data->IsRegistered = isRegistered;
			data->IsPointer = std::is_pointer<T>::value;
			data->DereferenceFunc = []() { return Type::Get<std::remove_pointer_t<T>>(); };

			return data;
		}

	private:
		static std::vector<TypeData*> s_TypeDatas;

	private:
		TypeData* m_Data;
		TypeFlags m_Flags;
	};
}

