#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include <iostream>
#include "Utils/Ref.h"

namespace RefLib
{
	class Property;
	class Method;
	class Constructor;
	class Variant;
	class Argument;
	class Enum;

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
		static bool RegisterType(const std::string& name, PropertyContainer* properties, MethodContainer* methods, std::vector<ConstructorData>* constructors)
		{
			Type t = Get<T>();
			if (t.IsRegistered())
				return false;

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true);
			data->Properties = properties;
			data->Methods = methods;
			data->Constructors = constructors;
			s_TypeDatas[data->Id] = data;
			return true;
		}

		template<typename T>
		static bool RegisterEnum(const std::string& name, EnumDataWrapper* enumData)
		{
			static_assert(std::is_enum_v<T>, "Type must be an enum");

			Type t = Get<T>();
			if (t.IsRegistered())
				return false;

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true);
			data->EnumValue = enumData;
			s_TypeDatas[data->Id] = data;
		}

	public:
		Type(TypeData* data, TypeFlags flags) : m_Data(data), m_Flags(flags) {}
		Type() : m_Data(nullptr), m_Flags(TypeFlags::None) {}
		Type(const Type& other) = default;
		Type& operator=(const Type& other) { this->m_Data = other.m_Data; this->m_Flags = other.m_Flags; return *this; }
		~Type() = default;

		std::string_view GetName() const { return IsValid() ? m_Data->Name : ""; }
		TypeId GetId() const { return IsValid() ? m_Data->Id : NoneType; }
		TypeFlags GetFlags() const { return m_Flags; }
		bool IsRegistered() const { return IsValid() ? m_Data->IsRegistered : false; }

		static Type Invalid() { return Type(nullptr, TypeFlags::None); }

		bool IsFlag(TypeFlags flag) const { return (bool)(m_Flags & flag); }
		bool IsConst() const { return IsFlag(TypeFlags::Const); }
		bool IsVolotile() const { return IsFlag(TypeFlags::Volatile); }
		bool IsRef() const { return IsFlag(TypeFlags::Reference); }
		bool IsRValueRef() const { return IsFlag(TypeFlags::RValueReference); }

		Property GetProperty(const std::string& name);
		const std::vector<Property>& GetProperties();

		Method GetMethod(const std::string& name);
		Method GetOverloadedMethod(const std::string& name, Type signature);
		std::vector<Method> GetMethods();

		Constructor GetConstructor(const std::vector<Type>& params);
		std::vector<Constructor> GetConstructors();
		Variant Create(std::vector<Argument> args);
		Variant CreatePtr(std::vector<Argument> args);

		bool IsEnum() const { return IsValid() ? m_Data->EnumValue != nullptr : false; }
		Enum AsEnum() const;

		bool IsPointer() const { return IsValid() ? m_Data->IsPointer : false; }
		 
		bool IsAssignableFrom(Type type) const; 
		bool IsConvertableTo(Type type) const; 

		Type Dereferenced() { return m_Data->DereferenceFunc(); }

		bool operator==(const Type& other) const
		{
			return GetId() == other.GetId() && m_Flags == other.GetFlags();
		}

		bool operator!=(const Type& other) const
		{
			return !(*this == other);
		}

		bool IsValid() const { return m_Data != nullptr; }

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

namespace std {

	template <>
	struct hash<RefLib::Type> 
	{
		std::size_t operator()(const RefLib::Type& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<size_t>()(k.GetId())
				^ (hash<uint8_t>()(static_cast<uint8_t>(k.GetFlags())) << 1)) >> 1);
		}
	};

}

