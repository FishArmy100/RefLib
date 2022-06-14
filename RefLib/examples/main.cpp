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

using namespace RefLib;

int main()
{
	auto builder = TypeBuilder<Player>("Player");
	builder.AddConstructor<const std::string&>();
	builder.AddConstructor<const Player&>();
	builder.AddProperty("m_Name", &Player::m_Name, AccessLevel::Private);
	builder.AddMethod("GetName", &Player::GetName, {});
	Type t = builder.Register();

	Variant var = std::move(t.Create({std::string("Default_Player_Name")}));
	std::cout << t.InvokeMethod(var, "GetName", {}).TryConvert<std::string>().value() << "\n";
}