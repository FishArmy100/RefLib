#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include <iostream>
#include "Utils/Ref.h"
#include "TypeDataPrototype.h"
#include <optional>
#include "Registration/TypeRegistrationFactory.h"
#include <unordered_set>
#include "Attributes/AttributeImpl.h"

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

		template<typename TSelectFunc>
		static std::vector<Type> Select(TSelectFunc func)
		{
			std::vector<Type> types;
			for (auto& data : s_TypeDatas)
			{
				Type t = Type(data, TypeFlags::None);
				if (func(t))
					types.push_back(t);
			}

			return types;
		}

		template<typename TIterFunc>
		static void Iterate(TIterFunc func)
		{
			for (auto& data : s_TypeDatas)
			{
				Type t = Type(data, TypeFlags::None);
				func(t);
			}
		}

		template<typename TFindFunc>
		static std::optional<Type> Find(TFindFunc func)
		{
			for (auto& data : s_TypeDatas)
			{
				Type t = Type(data, TypeFlags::None);
				if (func(t))
					return t;
			}

			return {};
		}

		template<typename T>
		static Type RegisterType(const std::string& name, TypeDataPrototype prototype, const std::vector<Variant>& attributes, std::optional<TypeId> declaring = {})
		{
			Type t = Get<T>();
			if (t.IsRegistered())
				throw std::exception("Cannot regester an already regestered type");

			bool isPointer = std::is_pointer_v<T>;
			TypeId id = t.GetId();
			std::function<Type()> dereferenceFunc = []() {return Type::Get<std::remove_pointer_t<T>>(); };
			TypeData* data = new TypeData(name, id, isPointer, prototype, attributes, dereferenceFunc, declaring);
			AppendDerivedTypes(t.GetData(), data->DerivedTypes);
			AddPreregisteredMethods(t.GetData(), data->Methods);

			delete s_TypeDatas[id];
			s_TypeDatas[id] = data;

			return Get<T>();
		}

		template<typename T>
		static Type RegisterEnum(const std::string& name, EnumDataWrapper* enumData, std::optional<TypeId> declaring)
		{
			static_assert(std::is_enum_v<T>, "Type must be an enum");

			Type t = Get<T>();
			if (t.IsRegistered())
				throw std::exception("Cannot regester an already regestered enum");

			std::function<Type()> dereferenceFunc = []() {return Type::Get<std::remove_pointer_t<T>>(); };
			TypeData* data = new TypeData(name, t.GetId(), enumData, dereferenceFunc, declaring);
			delete s_TypeDatas[data->Id];
			s_TypeDatas[data->Id] = data;

			return t;
		}

		template<typename TClass, typename TReturn, typename... TArgs>
		static void RegisterMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), std::vector<Type> templateArgs, AccessLevel level = AccessLevel::Public, const std::vector<std::string>& paramNames = {});

		template<typename T, typename TDerived>
		static void RegisterDerivedType()
		{
			static_assert(std::is_base_of_v<T, TDerived>, "TBase must be a base class of T");
			TypeId id = Get<T>().GetId();
			Type derived = Type::Get<TDerived>();
			AddDerivedTypeData(id, derived); 
		}

	public:
		Type(TypeId id, TypeFlags flags) : m_TypeId(id), m_Flags(flags) 
		{

		}

		Type(const Type& other) = default;
		Type& operator=(const Type& other) { this->m_TypeId = other.m_TypeId; this->m_Flags = other.m_Flags; return *this; }
		~Type() = default;

		const std::string& GetName() const { return GetData()->Name; }
		TypeId GetId() const { return GetData()->Id; }
		operator TypeId() const { return GetData()->Id; }
		TypeFlags GetFlags() const { return m_Flags; }
		bool IsRegistered() const { return GetData()->IsRegistered; }

		bool IsFlag(TypeFlags flag) const { return (bool)(m_Flags & flag); }
		bool IsConst() const { return IsFlag(TypeFlags::Const); }
		bool IsVolotile() const { return IsFlag(TypeFlags::Volatile); }
		bool IsRef() const { return IsFlag(TypeFlags::Reference); }
		bool IsRValueRef() const { return IsFlag(TypeFlags::RValueReference); }

		std::optional<Property> GetProperty(const std::string& name) const;
		std::vector<Property> GetProperties() const;

		Variant GetPropertyValue(Instance instance, const std::string& propName) const;
		bool SetPropertyValue(Instance instance, const std::string& propName, Argument arg) const;

		std::optional<Method> GetMethod(const std::string& name) const;
		std::optional<Method> GetMethod(const std::string& name, const std::vector<Type>& paramTypes) const;
		std::optional<Method> GetTemplatedMethod(const std::string& name, const std::vector<Type>& templateArgs, const std::vector<Type>& paramTypes) const;
		Variant InvokeMethod(Instance instance, const std::string& name, std::vector<Argument> args) const;
		Variant InvokeMethod(const std::string& name, std::vector<Argument> args) const;
		Variant InvokeTemplatedMethod(Instance instance, const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args) const;
		Variant InvokeTemplatedMethod(const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args) const;
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
		const std::vector<Type>& GetDerivedTypes() const;
		std::optional<Type> GetDerivedType(const std::string& name) const;

		std::optional<Type> GetNestedType(const std::string& name) const;
		const std::vector<Type>& GetNestedTypes() const;
		std::optional<Type> GetDeclaringType() const;
		bool IsNestedType() const;

		std::optional<ContainerView> AsContainer(Instance instance) const;
		bool IsContainer() const;

		bool IsEnum() const { return GetData()->EnumValue.has_value(); }
		std::optional<Enum> AsEnum() const; 

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*GetData()->Attributes); // is used for a bunch of attribute based methods

		bool IsPointer() const { return GetData()->IsPointer; }
		 
		bool IsAssignableFrom(Type type) const; 
		bool IsConvertableTo(Type type) const; 

		Type GetDereferenced() const { return GetData()->DereferenceFunc(); }

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
				std::function<Type()> dereferenceFunc = []() {return Type::Get<std::remove_pointer_t<T>>(); };
				s_TypeDatas.push_back(new TypeData(name, id, std::is_pointer_v<T>, dereferenceFunc));
			}

			Type type = Type(id, flags);

			// gard against stack overflow if try to get the same type while registering it
			if (!type.IsRegistered() && s_AutoRegisteringTypes.find(id) == s_AutoRegisteringTypes.end())
			{
				s_AutoRegisteringTypes.emplace(id);
				TypeRegistrationFactory<T> factory;
				bool registered = factory();

				s_AutoRegisteringTypes.erase(id);
			}

			return type;
		}

		static void AddPreregisteredMethods(Ref<TypeData> data, MethodContainer* container);
		static void AppendDerivedTypes(Ref<TypeData> data, DerivedTypeContainer* container);
		static void AddDerivedTypeData(TypeId id, Type derived);

	private:
		Ref<TypeData> GetData() const { return s_TypeDatas.at(m_TypeId); }

	private:
		static inline std::vector<TypeData*> s_TypeDatas{};
		static inline std::unordered_set<TypeId> s_AutoRegisteringTypes{};

	private:
		TypeId m_TypeId;
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

