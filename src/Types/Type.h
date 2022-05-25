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
	struct BaseType;

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
		static Type RegisterType(const std::string& name, TypeDataPrototype prototype, std::optional<TypeId> declaring = {})
		{
			if (!prototype.IsFullyBuilt())
				throw std::exception("Prototype is not fully built");

			Type t = Get<T>();
			if (t.IsRegistered())
				throw std::exception("Cannot regester an already regestered type");

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true, declaring);
			data->Properties = prototype.Properties;
			data->Methods = prototype.Methods;
			data->Constructors = prototype.Constructors;
			data->BaseTypes = prototype.BaseTypes;
			data->NestedTypes = prototype.NestedTypes;

			*s_TypeDatas[data->Id] = *data;

			return Get<T>();
		}

		template<typename T>
		static Type RegisterEnum(const std::string& name, EnumDataWrapper* enumData, std::optional<TypeId> declaring = {})
		{
			static_assert(std::is_enum_v<T>, "Type must be an enum");

			Type t = Get<T>();
			if (t.IsRegistered())
				throw std::exception("Cannot regester an already regestered enum");

			TypeData* data = CreateTypeData<T>(name, t.GetId(), true, declaring);
			data->EnumValue = enumData;
			s_TypeDatas[data->Id] = data;

			return t;
		}

	public:
		Type(TypeData* data, TypeFlags flags) : m_Data(data), m_Flags(flags) 
		{
			if (data == nullptr)
				throw std::exception("Cannot construct type from nullptr");
		}

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

		std::optional<Property> GetProperty(const std::string& name) const;
		const std::vector<Property>& GetProperties() const;

		Variant GetPropertyValue(Instance instance, const std::string& propName) const;
		bool SetPropertyValue(Instance instance, const std::string& propName, Argument arg) const;

		std::optional<Method> GetMethod(const std::string& name) const;
		std::optional<Method> GetMethod(const std::string& name, const std::vector<Type>& paramTypes) const;
		Variant InvokeMethod(Instance instance, const std::string& name, std::vector<Argument> args) const;
		Variant InvokeMethod(const std::string& name, std::vector<Argument> args) const;
		std::vector<Method> GetMethods() const;

		std::optional<Constructor> GetConstructor(const std::vector<Type>& params) const;
		std::vector<Constructor> GetConstructors() const;
		Variant Create(std::vector<Argument> args) const;
		Variant CreatePtr(std::vector<Argument> args) const;
		bool IsDefaultConstructable() const;

		std::optional<BaseType> GetBaseType(const std::string& name) const;
		const std::vector<BaseType>& GetBaseTypes() const;
		bool IsBaseOf(Type type) const;
		bool IsDerivedFrom(Type type) const;

		std::optional<Type> GetNestedType(const std::string& name) const;
		const std::vector<Type>& GetNestedTypes() const;
		std::optional<Type> GetDeclaringType();
		bool IsNestedType();

		bool IsEnum() const { return m_Data->EnumValue != nullptr; } 
		std::optional<Enum> AsEnum() const; 

		bool IsPointer() const { return m_Data->IsPointer; }
		 
		bool IsAssignableFrom(Type type) const; 
		bool IsConvertableTo(Type type) const; 

		Type Dereferenced() const { return m_Data->DereferenceFunc(); }

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
				s_TypeDatas.push_back(CreateTypeData<T>(name, id, false, {}));
			}

			return Type(s_TypeDatas[id], flags);
		}

		template<typename T>
		static TypeData* CreateTypeData(const std::string& name, TypeId id, bool isRegistered, std::optional<TypeId> declaring)
		{
			TypeData* data = new TypeData(name, id);
			data->IsRegistered = isRegistered;
			data->IsPointer = std::is_pointer<T>::value;
			data->DereferenceFunc = []() { return Type::Get<std::remove_pointer_t<T>>(); }; 
			data->DeclaringType = declaring;

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

