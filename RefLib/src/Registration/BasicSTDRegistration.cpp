#define RTTR_ENABLE_STD

#ifdef RTTR_ENABLE_STD 
#include "Registration/EnumBuilder.h"
#include "Registration/TypeRegistrationFactory.h"
#include "Registration/TypeBuilder.h"
#include "Container/ContainerViewIterator.h"

#include <vector>
#include <map>
#include <optional>
#include <memory>

namespace RefLib
{
	// -------------------------- String --------------------------

	struct TypeRegistrationFactory<std::string>
	{
		bool operator()()
		{
			auto builder = TypeBuilder<std::string>("std::string");
			builder.AddConstructor<>();
			builder.AddConstructor<const char*>();
			builder.AddConstructor<const std::string&>();

			builder.AddMethod("size", &std::string::size, AccessLevel::Public, {}, {});
			builder.AddMethod("c_str", &std::string::c_str, AccessLevel::Public, {}, {});
			builder.AddMethod("at", static_cast<char&(std::string::*)(const size_t)>(&std::string::at), AccessLevel::Public, {}, {});

			builder.Register();

			return true;
		}
	};

	// -------------------------- Vector --------------------------
	template<typename T>
	class VectorContainerView : public IContainerView
	{
	public:
		VectorContainerView(std::vector<T>& vec) : m_Vec(vec) {}

		ContainerViewIterator begin() override
		{
			return ContainerViewIterator(m_Vec->begin());
		}

		ContainerViewIterator end() override
		{
			return ContainerViewIterator(m_Vec->end());
		}

		size_t Length() override
		{
			return m_Vec->size();
		}

		Type GetType() override
		{
			return Type::Get<std::vector<T>>();
		}

		Type GetElementType() override
		{
			return Type::Get<T>();
		}

	private:
		Ref<std::vector<T>> m_Vec;
	};

	template<typename T>
	struct TypeRegistrationFactory<std::vector<T>>
	{
		bool operator()()
		{
			Type contained = Type::Get<T>();
			auto builder = TypeBuilder<std::vector<T>>("std::vector<" + contained.GetName() + ">");
			builder.AddConstructor<>();
			builder.AddMethod("size", &std::vector<T>::size, AccessLevel::Public, {}, {});
			builder.AddMethod("at", static_cast<T&(std::vector<T>::*)(size_t)>(&std::vector<T>::at), AccessLevel::Public, { "index" });
			builder.AddMethod("push_back", static_cast<void(std::vector<T>::*)(const T&)>(&std::vector<T>::push_back), AccessLevel::Public, { "item" });
			builder.SetAsContainer(
				[](std::vector<T>& t) -> ContainerView
				{
					return ContainerView(std::make_shared<VectorContainerView<T>>(t));
				}
			);
			builder.Register();
			return true;
		}
	};

	// -------------------------- Array --------------------------
	template<typename T, int size>
	class ArrayContainerView : public IContainerView
	{
		using Array = std::array<T, size>;
	public:
		ArrayContainerView(Array& arr) : m_Array(arr) {}

		ContainerViewIterator begin() override
		{
			return ContainerViewIterator(m_Array->begin());
		}

		ContainerViewIterator end() override
		{
			return ContainerViewIterator(m_Array->end());
		}

		size_t Length() override
		{
			return m_Array->size();
		}

		Type GetType() override
		{
			return Type::Get<Array>();
		}

		Type GetElementType() override
		{
			return Type::Get<T>();
		}

	private:
		Ref<Array> m_Array; 
	};

	template<typename T, size_t size>
	struct TypeRegistrationFactory<std::array<T, size>>
	{
		using Array = std::array<T, size>;

		bool operator()()
		{
			Type contained = Type::Get<T>();
			auto builder = TypeBuilder<Array>("std::array<" + contained.GetName() + ", " + std::to_string(size) + ">");
			builder.AddConstructor<>();
			builder.AddMethod("size", &Array::size);
			builder.AddMethod("data", static_cast<T*(Array::*)()>(&Array::data));
			builder.AddMethod("at", static_cast<T&(Array::*)(size_t)>(&Array::at));

			builder.SetAsContainer([](Array& arr) {return ContainerView(std::make_shared<ArrayContainerView<T, size>>(arr)); });

			builder.Register();
			return true;
		}
	};

	// -------------------------- Map --------------------------
	template<typename TFirst, typename TSecond>
	struct TypeRegistrationFactory<std::pair<TFirst, TSecond>>
	{
		using Pair = std::pair<TFirst, TSecond>;
		bool operator()()
		{
			Type firstType = Type::Get<TFirst>();
			Type secondType = Type::Get<TSecond>();

			auto builder = TypeBuilder<Pair>("std::pair<" + firstType.GetName() + ", " + secondType.GetName() + ">");
			builder.AddConstructor<const TFirst&, const TSecond&>();
			builder.AddProperty("first", &Pair::first);
			builder.AddProperty("second", &Pair::second);
			builder.Register();

			return true;
		}
	};

	template<typename TKey, typename TVal>
	class MapContainerView : public IContainerView
	{
		using Map = std::map<TKey, TVal>;
		using Pair = std::pair<TKey, TVal>;

	public:
		MapContainerView(Map& arr) : m_Map(arr) {}

		ContainerViewIterator begin() override
		{
			return ContainerViewIterator(m_Map->begin());
		}

		ContainerViewIterator end() override
		{
			return ContainerViewIterator(m_Map->end());
		}

		size_t Length() override
		{
			return m_Map->size();
		}

		Type GetType() override
		{
			return Type::Get<Map>();
		}

		Type GetElementType() override
		{
			return Type::Get<Pair>();
		}

	private:
		Ref<Map> m_Map;
	};

	template<typename TKey, typename TVal>
	struct TypeRegistrationFactory<std::map<TKey, TVal>>
	{
		using Pair = std::pair<TKey, TVal>;
		using Map = std::map<TKey, TVal>;

		bool operator()()
		{
			Type keyType = Type::Get<TKey>();
			Type valType = Type::Get<TVal>(); 

			auto builder = TypeBuilder<Map>("std::map<" + keyType.GetName() + ", " + valType.GetName() + ">");
			
			builder.AddConstructor<>();
			builder.AddConstructor<const Map&>();

			builder.AddMethod("size", &Map::size);
			builder.AddMethod("at", static_cast<TVal&(Map::*)(const TKey&)>(&Map::at), AccessLevel::Public, { "index" });
			builder.AddMethod("insert", static_cast<std::pair<Map::iterator, bool>(Map::*)(const Pair&)>(&Map::insert), AccessLevel::Public, { "value" });
			
			builder.SetAsContainer([](Map& map) { return ContainerView(std::make_shared<MapContainerView<TKey, TVal>>(map)); });

			builder.Register();
			return true;
		}
	};

	// -------------------------- Map --------------------------
	template<typename T>
	struct TypeRegistrationFactory<std::optional<T>>
	{
		using Opt = std::optional<T>;
		bool operator()()
		{
			auto builder = TypeBuilder<Opt>("std::optional<" + Type::Get<T>().GetName() + ">");
			builder.AddConstructor<>();
			builder.AddConstructor<const T&>();

			auto ptr = reinterpret_cast<T&(Opt::*)()>(static_cast<T&(Opt::*)()&>(&Opt::value));
			builder.AddMethod("value", ptr);
			builder.AddMethod("has_value", &Opt::has_value);
			builder.Register();
			return true;
		}
	};
}

#endif