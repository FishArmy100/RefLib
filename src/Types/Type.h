#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include <iostream>
#include "Utils/Ref.h"
#include "TypeDataPrototype.h"
#include <optional>

namespace RefLib
{
	class Property;
	class Method;
	class Constructor;
	class Variant;
	class Argument;
	class Enum;
	class Instance;

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

		template<typename... TArgs>
		static std::vector<Type> GetMultiple() { return std::vector<Type> { Get<TArgs>()... }; }

		static std::optional<Type> Get(const std::string& name);
		static std::optional<Type> Get(TypeId id);

		template<typename T>
		static bool RegisterType(const std::string& name, TypeDataPrototype prototype)
		{
			if (!prototype.IsFullyBuilt())
				return false;

			Type t = Get<T>();
			if (t.IsRegistered())
				return false;

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true);
			data->Properties = prototype.Properties;
			data->Methods = prototype.Methods;
			data->Constructors = prototype.Constructors;

			*s_TypeDatas[data->Id] = *data;
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
		Type(const Type& other) = default;
		Type& operator=(const Type& other) { this->m_Data = other.m_Data; this->m_Flags = other.m_Flags; return *this; }
		~Type() = default;

		std::string_view GetName() const { return m_Data->Name; }
		TypeId GetId() const { return m_Data->Id; }
		TypeFlags GetFlags() const { return m_Flags; }
		bool IsRegistered() const { return m_Data->IsRegistered; }

		bool IsFlag(TypeFlags flag) const { return (bool)(m_Flags & flag); }
		bool IsConst() const { return IsFlag(TypeFlags::Const); }
		bool IsVolotile() const { return IsFlag(TypeFlags::Volatile); }
		bool IsRef() const { return IsFlag(TypeFlags::Reference); }
		bool IsRValueRef() const { return IsFlag(TypeFlags::RValueReference); }

		std::optional<Property> GetProperty(const std::string& name);
		const std::vector<Property>& GetProperties();

		Variant GetPropertyValue(Instance instance, const std::string& propName);
		bool SetPropertyValue(Instance instance, const std::string& propName, Argument arg);

		std::optional<Method> GetMethod(const std::string& name);
		std::optional<Method> GetMethod(const std::string& name, const std::vector<Type>& paramTypes);
		Variant InvokeMethod(Instance instance, const std::string& name, std::vector<Argument> args);
		std::vector<Method> GetMethods();

		std::optional<Constructor> GetConstructor(const std::vector<Type>& params);
		std::vector<Constructor> GetConstructors();
		Variant Create(std::vector<Argument> args);
		Variant CreatePtr(std::vector<Argument> args);

		bool IsEnum() const { return m_Data->EnumValue != nullptr; }
		std::optional<Enum> AsEnum() const;

		bool IsPointer() const { return m_Data->IsPointer; }
		 
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

