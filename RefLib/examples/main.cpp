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
#include "Registration/HelperRegistrationMacros.h"
#include "Registration/BasicSTDRegistration.h"

struct Dammageable {};

struct Entity
{
	enum class EntityType { Friendly, Nutral, Enemy };
	EntityType EType;
	float Health;

	void PrintHealth() { std::cout << "Entity health: " << Health << "\n"; }
	REFLIB_FRIEND
};

class Player : public Entity
{
public:
	Player(const std::string& name) : m_Name(name) {}
	Player(const Player&) = default;
	~Player() = default;
	const std::string& GetName() const { return m_Name; }

private:
	std::string m_Name;

	friend int main();
	REFLIB_FRIEND
};

using namespace RefLib;

struct Test
{
	template<typename T>
	void Print(const T& val) const { std::cout << val << "\n"; }
};

int main()
{
	Variant opt = std::optional<int>(5);
	Type optType = opt.GetType();
	Variant value = optType.InvokeMethod(opt, "value", {});
	std::cout << value.TryConvert<int>().value() << "\n";

	Variant map = std::map<int, std::string>({ { 4, "Hello World!" } });
	std::cout << map.GetType().GetName() << "\n";
	Type mapType = map.GetType(); 
	Variant message = mapType.InvokeMethod(map, "at", { 4 });
	std::cout << message.TryConvert<std::string>().value(); 
}

REFLIB_REGISTRATION
{
	REFLIB_BEGIN_CLASS(Dammageable)
	REFLIB_END_CLASS()

	REFLIB_BEGIN_CLASS(Entity)
		REFLIB_PROP_BASIC(EType)
		REFLIB_PROP_BASIC(Health)
		REFLIB_METH_BASIC(void, PrintHealth, ())
		REFLIB_ATTRIBUTE(Dammageable{})
		REFLIB_BEGIN_NESTED_ENUM(EntityType)
			REFLIB_ENUM_VAL(Enemy)
			REFLIB_ENUM_VAL(Nutral)
			REFLIB_ENUM_VAL(Friendly)
			REFLIB_ATTRIBUTE(int{3})
		REFLIB_END_NESTED_ENUM()
	REFLIB_END_CLASS()

	REFLIB_BEGIN_CLASS(Player)
		REFLIB_BASE_CLASS(Entity)
		REFLIB_CTOR(const std::string&)
		REFLIB_CTOR(const Player&)
		REFLIB_PROP_ACCESS(m_Name, AccessLevel::Private)
		REFLIB_CONST_METH_BASIC(const std::string&, GetName, ())
	REFLIB_END_CLASS()

	REFLIB_BEGIN_CLASS(Test)
		REFLIB_CONST_METH_TEMP_BASIC(void, Print, (const int&), <int>)
		REFLIB_CONST_METH_TEMP_BASIC(void, Print, (const float&), <float>)
	REFLIB_END_CLASS();
}