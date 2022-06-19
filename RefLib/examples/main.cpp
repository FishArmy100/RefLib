#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Instance/Instance.h"
#include "Variant/Variant.h"
#include "Property/Property.h"
#include "Registration/TypeBuilder.h"
#include "Registration/EnumBuilder.h"
#include "Method/Method.h"
#include "Constructor/Constructor.h"
#include "Enum/Enum.h"
#include "Enum/EnumData.h"
#include "Types/MethodRegistration.h"
#include "Function/Function.h"
#include "Container/ContainerView.h"
#include "Container/ContainerViewIterator.h"

class Player
{
public:
	Player(const std::string& name) : m_Name(name) {}
	Player(const Player&) = default;
	~Player() = default;
	const std::string& GetName() const { return m_Name; }
private:
	std::string m_Name;
	friend int main();
};

namespace RefLib
{
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
}

namespace RefLib
{
	template<typename T>
	struct TypeRegistrationFactory<std::vector<T>>
	{
		bool operator()()
		{
			Type contained = Type::Get<T>();
			auto builder = TypeBuilder<std::vector<T>>("std::vector<" + contained.GetName() + ">");
			builder.AddConstructor<>();
			builder.AddMethod("at", static_cast<const T&(std::vector<T>::*)(size_t) const>(&std::vector<T>::at), {}, AccessLevel::Public, { "index" }, {});
			builder.AddMethod("push_back", static_cast<void(std::vector<T>::*)(const T&)>(&std::vector<T>::push_back), {}, AccessLevel::Public, { "item" }, {});
			builder.SetAsContainer(
				[](std::vector<T>& t) -> ContainerView 
				{
					return ContainerView(std::make_shared<VectorContainerView<int>>(t));
				}
			);
			builder.Register();
			return true;
		}
	};
}

using namespace RefLib;

int main()
{
	//auto builder = TypeBuilder<Player>("Player");
	//builder.AddConstructor<const std::string&>();
	//builder.AddConstructor<const Player&>();
	//builder.AddProperty("m_Name", &Player::m_Name, AccessLevel::Private);
	//builder.AddMethod("GetName", &Player::GetName, {});
	//builder.Register();

	Type t = Type::Get<std::vector<int>>();
	Variant v = t.Create({});
	t.InvokeMethod(v, "push_back", { 5 });
	t.InvokeMethod(v, "push_back", { 7 });

	for (Instance i : v.GetContainerView())
	{
		std::cout << *i.TryConvert<int>() << "\n";
	}
}